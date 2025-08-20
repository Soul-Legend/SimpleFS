#include "fs.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

int INE5412_FS::fs_format() {
  // Caso disco já esteja montado, não formata
  if (getIsMounted()) {
    return 0;
  }

  // Limpa o conteúdo do superbloco
  union fs_block superblock;
  std::fill(superblock.data, superblock.data + Disk::DISK_BLOCK_SIZE, 0);

  int ninodeblocks = std::ceil(disk->size() * 0.1);

  // Constrói superbloco
  superblock.super.magic = FS_MAGIC;
  superblock.super.nblocks = disk->size();
  superblock.super.ninodeblocks = ninodeblocks;
  superblock.super.ninodes = ninodeblocks * INODES_PER_BLOCK;

  // Escreve superbloco no disco
  disk->write(0, superblock.data);

  // Libera tabela de inodes
  union fs_block inode_block;
  std::fill(inode_block.data, inode_block.data + Disk::DISK_BLOCK_SIZE, 0);
  for (int block_i = 1; block_i <= ninodeblocks; ++block_i) {
    disk->write(block_i, inode_block.data);
  }

  return 1;
}

std::string INE5412_FS::get_superblock_info() const {
  std::stringstream ss;

  union fs_block superblock;

  // Le superbloco e armazena seu conteudo de debug em uma stringstream
  disk->read(0, superblock.data);

  ss << "    magic number is "
     << (superblock.super.magic == FS_MAGIC ? "valid" : "invalid!") << '\n';
  ss << "    " << superblock.super.nblocks << " blocks\n";
  ss << "    " << superblock.super.ninodeblocks << " inode blocks\n";
  ss << "    " << superblock.super.ninodes << " inodes\n";

  return ss.str();
}

std::vector<int> INE5412_FS::get_inode_list() const {
  std::vector<int> inodes;

  // Le superbloco
  union fs_block superblock;
  disk->read(0, superblock.data);

  // Percorre todos os blocos de inodes
  for (int block_i = 1; block_i <= superblock.super.ninodeblocks; ++block_i) {
    union fs_block inode_block;
    disk->read(block_i, inode_block.data);

    // Percorre todos os inodes de um bloco
    for (int inode_i = 0; inode_i < INODES_PER_BLOCK; ++inode_i) {
      fs_inode &inode = inode_block.inode[inode_i];

      // Caso seja inválido, pula
      if (!inode.isvalid) {
        continue;
      }

      // Caso seja válido, adiciona a lista de inodes
      int inumber = inode_i + 1 + INODES_PER_BLOCK * (block_i - 1);
      inodes.push_back(inumber);
    }
  }

  return inodes;
}

std::string INE5412_FS::get_inode_info(int inumber) {

  // Carrega inode
  fs_inode inode;
  inode_load(inumber, &inode);

  std::stringstream ss;
  ss << "    size: " << inode.size << " bytes\n";

  // Percorre blocos diretos e os adiciona a stringstream caso forem válidos
  ss << "    direct blocks:";
  for (int direct_i = 0; direct_i < POINTERS_PER_INODE; ++direct_i) {
    int block_num = inode.direct[direct_i];

    if (block_num == 0) {
      continue;
    }

    ss << ' ' << block_num;
  }
  ss << '\n';

  // Caso o inodo possua um bloco indireto, percorre
  if (inode.indirect != 0) {
    ss << "    indirect block: " << inode.indirect << '\n';

    union fs_block indirect_block;
    disk->read(inode.indirect, indirect_block.data);

    ss << "    indirect data blocks:";

    // Adiciona a stringstream blocos indiretos caso forem válidos
    for (int indirect_i = 0; indirect_i < POINTERS_PER_BLOCK; ++indirect_i) {
      int block_num = indirect_block.pointers[indirect_i];

      if (block_num == 0) {
        continue;
      }

      ss << ' ' << block_num;
    }
    ss << '\n';
  }

  return ss.str();
}

bool INE5412_FS::getIsMounted() {
  return isMounted;
}

void INE5412_FS::fs_debug() {
  if (!getIsMounted()) {
    std::cout << "filesystem is not mounted\n";
    return;
  }

  // Obtem informação do superbloco
  std::cout << "superblock:\n";
  std::cout << get_superblock_info();

  // Percorre todos os inodes do sistema e obtem suas informações
  for (int inumber : get_inode_list()) {
    std::cout << "inode " << inumber << ":\n";
    std::cout << get_inode_info(inumber);
  }
}

int INE5412_FS::fs_mount() {

  // Caso já esteja montando, não monta
  if (getIsMounted()) {
    std::cout << "filesystem already mounted\n";
    return 0;
  }

  // Le superbloco
  union fs_block superblock;
  disk->read(0, superblock.data);


  // Caso sistema de arquivo seja invalido, não monta
  if (superblock.super.magic != FS_MAGIC) {
    return 0;
  }

  // No início, será assumido que todos os blocos estão livres.
  bitmap = std::vector<bool>(superblock.super.nblocks, true);
  bitmap[0] = false;

  // Percorre toda a tabela de inodos
  for (int block_i = 1; block_i <= superblock.super.ninodeblocks; ++block_i) {
    // Bloco de inode é marcado como ocupado
    bitmap[block_i] = false;

    // Le o bloco de inodes
    union fs_block inode_block;
    disk->read(block_i, inode_block.data);

    // Percorre todos os inodes do bloco
    for (int inode_i = 0; inode_i < INODES_PER_BLOCK; ++inode_i) {
      fs_inode &inode = inode_block.inode[inode_i];

      // Se o inode não é válido, pula
      if (!inode.isvalid) {
        continue;
      }

      // Percorre blocos diretos do inodo
      for (int direct_i = 0; direct_i < POINTERS_PER_INODE; ++direct_i) {
        int block_num = inode.direct[direct_i];

        // Caso o ponteiro do bloco seja válido, bloco é marcado como ocupado
        if (block_num != 0) {
          bitmap[block_num] = false;
        }
      }

      // Caso o inode possua um bloco de ponteiros indiretos, percorre
      if (inode.indirect != 0) {

        // Marca bloco de ponteiros indiretos como ocupado no bitmap
        bitmap[inode.indirect] = false;

        // Le bloco de ponteiros indiretos
        union fs_block indirect_block;
        disk->read(inode.indirect, indirect_block.data);

        // Para cada ponteiro indireto, verifica se o ponteiro é válido
        for (int indirect_i = 0; indirect_i < POINTERS_PER_BLOCK;
             ++indirect_i) {
          int block_num = indirect_block.pointers[indirect_i];

          // Caso o ponteiro seja válido, marca o bloco como ocupado no bitmap
          if (block_num != 0) {
            bitmap[block_num] = false;
          }
        }
      }
    }
  }

  isMounted = true;

  return 1;
}

int INE5412_FS::fs_create() {

  // Caso disco não esteja montado, falha e retorna 0
  if (!getIsMounted()) {
    return 0;
  }

  union fs_block superblock;
  disk->read(0, superblock.data);

  // Passa por todos os blocos de inodes
  for (int block_i = 1; block_i <= superblock.super.ninodeblocks; ++block_i) {
    union fs_block inode_block;
    disk->read(block_i, inode_block.data);

    // Procura por um inode inválido
    for (int inode_i = 0; inode_i < INODES_PER_BLOCK; ++inode_i) {
      fs_inode &inode = inode_block.inode[inode_i];

      if (inode.isvalid) {
        continue;
      }

      // Ao encontrar um inode inválido, prepara o inode
      inode.isvalid = true;
      inode.size = 0;
      for (int direct_i = 0; direct_i < POINTERS_PER_INODE; direct_i++) {
        inode.direct[direct_i] = 0;
      }
      inode.indirect = 0;

      // Atualiza bloco que armazena o inode
      disk->write(block_i, inode_block.data);
      return (block_i - 1) * INODES_PER_BLOCK + inode_i + 1;
    }
  }

  // Caso não encontre um inode livre, falha e retorna 0
  return 0;
}

int INE5412_FS::fs_delete(int inumber) {

  // Caso não esteja montando, ou o inumero é inválido, não deleta
  if (!getIsMounted() || inumber < 1) {
    return 0;
  }

  // Carrega o inode de inumero igual a inumber
  fs_inode inode;
  inode_load(inumber, &inode);

  // Se for inválido, não deleta
  if (!inode.isvalid) {
    return 0;
  }

  // Limpa dados do inode
  inode.isvalid = false;
  inode.size = 0;

  // Libera blocos diretos
  for (int direct_i = 0; direct_i < POINTERS_PER_INODE; ++direct_i) {
    int block_num = inode.direct[direct_i];

    if (block_num != 0) {
      bitmap[block_num] = true;
    }
    inode.direct[direct_i] = 0;
  }

  // Libera blocos indiretos
  if (inode.indirect != 0) {
    bitmap[inode.indirect] = true;

    union fs_block indirect_block;
    disk->read(inode.indirect, indirect_block.data);

    for (int indirect_i = 0; indirect_i < POINTERS_PER_BLOCK; ++indirect_i) {
      int block_num = indirect_block.pointers[indirect_i];

      if (block_num != 0) {
        bitmap[block_num] = true;
      }
    }

    inode.indirect = 0;
  }

  // Atualiza bloco que armazena o inode
  inode_write(inumber, inode);
  return 1;
}

int INE5412_FS::fs_getsize(int inumber) {

  // Caso não esteja montado, retorna -1
  if (!getIsMounted()) {
    return -1;
  }

  // Carrega o inode de inumero igual a inumber
  fs_inode inode;
  inode_load(inumber, &inode);

  // Se o inode for inválido, retorna -1
  if (!inode.isvalid) {
    return -1;
  }

  // Retorna tamanho
  return inode.size;
}

int INE5412_FS::fs_read(int inumber, char *data, int length, int offset) {
  // Caso não esteja montado ou o inumber seja menor que 1, não le
  if (!getIsMounted() || inumber < 1) {
    return 0;
  }

  // Obter inode
  fs_inode inode;
  inode_load(inumber, &inode);

  // Caso o offset seja maior do que o tamanho do inode, retorna 0
  if (!inode.isvalid || offset >= inode.size) {
    return 0;
  }

  // Determina indice dos blocos a serem lidos
  int block_num = offset / Disk::DISK_BLOCK_SIZE;
  int final_indx = std::min((offset + length - 1), inode.size - 1);
  int final_block_num = final_indx / Disk::DISK_BLOCK_SIZE;

  int data_indx = 0;
  int block_pointer;
  union fs_block datablock, indirectblock;
  bool already_read = false;

  // Passa pelos blocos relacionados ao inode
  for (; block_num <= final_block_num; block_num++) {

    // Verifica se o bloco atual é um bloco direto ou indireto
    // e obtem seu ponteiro
    if (block_num < POINTERS_PER_INODE) {
      block_pointer = inode.direct[block_num];
    } else {
      if (!already_read) {
        disk->read(inode.indirect, indirectblock.data);
        already_read = true;
      }
      block_pointer = indirectblock.pointers[block_num - POINTERS_PER_INODE];
    }

    // Le bloco do disco
    disk->read(block_pointer, datablock.data);

    // Le bloco até o fim do inode ou até o fim do bloco
    for (int datablock_indx = 0;
         offset + data_indx <= final_indx &&
         datablock_indx < static_cast<int>(Disk::DISK_BLOCK_SIZE);
         datablock_indx++, data_indx++) {
      data[data_indx] = datablock.data[datablock_indx];
    }
  }

  // Ao final do laço data_indx corresponde a quantidade de bytes lidos
  return data_indx;
}

int INE5412_FS::claim_free_block() {

  // Caso não esteja montado, retorna -1
  if (!getIsMounted()) {
    return -1;
  }

  // Percorre o bitmap até encontrar um bloco livre ou até exceder o tamanho do
  // disco.
  int block_i = 0;
  for (; block_i < disk->size() && !bitmap[block_i]; ++block_i)
    ;

  // O disco está cheio.
  if (block_i == disk->size()) {
    return -1;
  }

  // O bloco com índice block_i foi reinvidicado e ele não está mais livre.
  bitmap[block_i] = false;

  return block_i;
}

int INE5412_FS::fs_write(int inumber, const char *data, int length,
                         int offset) {

  // Caso disco não esteja montado ou o inumero seja inválido, retorna 0
  if (!getIsMounted() || inumber < 1) {
    return 0;
  }

  fs_inode inode;
  inode_load(inumber, &inode);

  if (!inode.isvalid) {
    std::cout << "invalid inode\n";
    return 0;
  }

  // Determina indice dos blocos internos ao inodo a serem escritos
  int first_block_num = offset / Disk::DISK_BLOCK_SIZE;
  int final_block_num = (offset + length - 1) / Disk::DISK_BLOCK_SIZE;

  union fs_block indirect_block;
  bool already_read_indirect_block = false;
  int num_bytes_written = 0;
  
  // Percorre os blocos internos ao inode
  for (int block_num = first_block_num, data_i = 0;
       block_num <= final_block_num;
       ++block_num, data_i += Disk::DISK_BLOCK_SIZE) {
    int block_pointer;
    // Caso o bloco pertenca aos blocos diretos
    if (block_num < POINTERS_PER_INODE) {
      // Se o ponteiro para o bloco direto for nulo, tenta alocar
      if (inode.direct[block_num] == 0) {
        int free_block = claim_free_block();
        // Caso não exista blocos livres no disco, interrompe a escrita
        if (free_block == -1) {
          break;
        }
        // Caso encontre um bloco livre, guarda seu ponteiro
        inode.direct[block_num] = free_block;
      }
      // Ponteiro de bloco atual é obtido
      block_pointer = inode.direct[block_num];
    } else {
      // Cria o bloco indireto caso ele não exista.
      if (inode.indirect == 0) {
        int free_block = claim_free_block();
        if (free_block == -1) {
          break;
        }
        inode.indirect = free_block;
      }

      // Caso a leitura do bloco indireto já tenha ocorrido, não repete
      if (!already_read_indirect_block) {
        disk->read(inode.indirect, indirect_block.data);
        already_read_indirect_block = true;
      }

      // Caso o bloco indireto atual tenha um ponteiro nulo, tenta alocar
      int pointer_i = block_num - POINTERS_PER_INODE;
      if (indirect_block.pointers[pointer_i] == 0) {
        int free_block = claim_free_block();
        // Caso não exista blocos livres no disco, interrompe a escrita
        if (free_block == -1) {
          break;
        }
        indirect_block.pointers[pointer_i] = free_block;
      }
      // Ponteiro de bloco atual é obtido
      block_pointer = indirect_block.pointers[pointer_i];
    }

    int increase_bytes = std::min(int(Disk::DISK_BLOCK_SIZE), length - data_i);

    // Preenche bloco de dados a ser escrito
    union fs_block to_write;
    std::fill(to_write.data, to_write.data + Disk::DISK_BLOCK_SIZE, 0);
    std::copy(data + data_i, data + data_i + increase_bytes, to_write.data);

    // Escreve bloco de dados e incrimenta tamanho do inode
    disk->write(block_pointer, to_write.data);
    num_bytes_written += increase_bytes;
  }

  if (offset == 0) {
    inode.size = num_bytes_written;
  } else {
    inode.size += num_bytes_written;
  }

  // Escreve o inodo atualizado.
  inode_write(inumber, inode);
  // Escreve o bloco indireto atualizado caso ele foi usado.
  if (already_read_indirect_block) {
    disk->write(inode.indirect, indirect_block.data);
  }

  if (num_bytes_written < length) {
    std::cout << "The disk is full.\n";
  }

  return num_bytes_written;
}

void INE5412_FS::inode_load(int inumber, fs_inode *inode) {
  union fs_block inode_block;

  int block_i = (inumber / INODES_PER_BLOCK) + 1;
  disk->read(block_i, inode_block.data);

  int inode_i = (inumber - 1) % INODES_PER_BLOCK;
  *inode = inode_block.inode[inode_i];
  return;
}

void INE5412_FS::inode_write(int inumber, const fs_inode &inode) {
  union fs_block inode_block;
  int block_i = ((inumber - 1) / INODES_PER_BLOCK) + 1;
  disk->read(block_i, inode_block.data);

  int inode_i = (inumber - 1) % INODES_PER_BLOCK;
  inode_block.inode[inode_i] = inode;

  disk->write(block_i, inode_block.data);
}

bool INE5412_FS::fs_is_disk_full() const {
  return std::none_of(bitmap.begin(), bitmap.end(), [](auto u) { return u; });
}
