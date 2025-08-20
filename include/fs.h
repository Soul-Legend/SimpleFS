#ifndef FS_H
#define FS_H

#include "disk.h"

#include <string>
#include <vector>


/**
 * @class INE5412_FS
 * @brief Classe que representa um sistema de arquivos simplificado.
 *
 * A classe INE5412_FS fornece uma implementação básica de um sistema de arquivos,
 * incluindo operações para formatação, montagem, criação, exclusão, leitura e escrita de inodes.
 * 
 * A classe contém definições para o superbloco, inodes e blocos de dados, além de métodos
 * para manipulação desses componentes. Ela também gerencia o estado de montagem do sistema
 * de arquivos e um bitmap para rastrear blocos livres.
 */
class INE5412_FS {
public:
  static const unsigned int FS_MAGIC = 0xf0f03410;
  static const unsigned short int INODES_PER_BLOCK = 128;
  static const unsigned short int POINTERS_PER_INODE = 5;
  static const unsigned short int POINTERS_PER_BLOCK = 1024;

  class fs_superblock {
  public:
    unsigned int magic;
    int nblocks;
    int ninodeblocks;
    int ninodes;
  };

  class fs_inode {
  public:
    int isvalid;
    int size;
    int direct[POINTERS_PER_INODE];
    int indirect;
  };

  union fs_block {
  public:
    fs_superblock super;
    fs_inode inode[INODES_PER_BLOCK];
    int pointers[POINTERS_PER_BLOCK];
    char data[Disk::DISK_BLOCK_SIZE];
  };

public:

  /**
   * @brief Construtor da classe INE5412_FS.
   * @param d Ponteiro para o disco.
   */
  INE5412_FS(Disk *d) {
    disk = d;
    isMounted = false;
  }

  /**
   * @brief Depura o sistema de arquivos.
   * 
   * Percorre o superbloco e os inodos do sistema e exibe suas
   * informações.
   */
  void fs_debug();

  /**
   * @brief Formata o sistema de arquivos.
   * 
   * Escreve superbloco e libera tabela de inodos.
   * 
   * @return int 1 em caso de sucesso, 0 em caso de falha.
   */
  int fs_format();

  /**
   * @brief Monta o sistema de arquivos e forma o bitmap.
   * @return int 1 em caso de sucesso, 0 em caso de falha.
   */
  int fs_mount();

  /**
   * @brief Cria um novo inodo de comprimento zero.
   * @return int Inumero do inodo criado ou zero em caso de falha.
   */
  int fs_create();

  /**
   * @brief Deleta o inodo de inumero igual a inumber.
   * @param inumber Inumero do inodo a ser deletado.
   * @return int 1 em caso de sucesso, 0 caso falhe.
   */
  int fs_delete(int inumber);

  /**
   * @brief Obtem tamanho em bytes do inode de inumero igual a inumber.
   * @param inumber Inumero do inodo.
   * @return int tamanho em bytes em caso de sucesso, -1 caso falhe.
   */
  int fs_getsize(int inumber);

  /**
   * @brief Le “length” bytes a partir de “offset” de um inodo valido e armazena
   * em data.
   * @param inumber Inumero do inodo a ser lido.
   * @param data Ponteiro para onde os dados lidos serão armazenados.
   * @param length Numero de bytes a serem lidos.
   * @param offset Offset do inodo a partir do qual os dados serão lidos.
   * @return int Numero de bytes lidos, 0 caso falhe.
   */
  int fs_read(int inumber, char *data, int length, int offset);

  /**
   * @brief Escreve “length” bytes a partir de “offset” em um inodo valido.
   * @param inumber Inumero do inodo a ser escrito.
   * @param data Ponteiro para os dados a serem escritos.
   * @param length Numero de bytes a serem escritos.
   * @param offset Offset do inodo a partir do qual os dados serão escritos.
   * @return int Numero de bytes escritos, 0 caso falhe.
   */
  int fs_write(int inumber, const char *data, int length, int offset);

  /**
   * @brief Obtem informações do superbloco.
   * @return std::string Informações do superbloco.
   */
  std::string get_superblock_info() const;

  /**
   * @brief Obtem a lista de inodes.
   * @return std::vector<int> Lista de inodes.
   */
  std::vector<int> get_inode_list() const;

  /**
   * @brief Obtem informações de debug de um inode.
   * @param inumber Inumero do inodo.
   * @return std::string Informações do inodo.
   */
  std::string get_inode_info(int inumber);

  /**
   * @brief Verifica se o sistema de arquivos está montado.
   * @return bool true se estiver montado, false caso contrário.
   */
  bool getIsMounted();

  /**
   * @brief Verifica se o disco está cheio.
   * @return bool true se o disco estiver cheio, false caso contrário.
   */
  bool fs_is_disk_full() const;

private:
  Disk *disk;
  bool isMounted;
  std::vector<bool> bitmap;

private:

  /**
   * @brief Carrega um inode de inumero igual a inumber a partir do disco
   * e armazena em *inode.
   * @param inumber Inumero do inodo a ser lido.
   * @param inode Ponteiro para onde o inode lido será armazenado.
   */
  void inode_load(int inumber, fs_inode *inode);

  /**
  * @brief Escreve um inodo em um inúmero específico.
  * @param inumber Inúmero no qual o inodo será escrito.
  * @param inode Ponteiro para o inodo que deve ser escrito.
  */
  void inode_write(int inumber, const fs_inode &inode);

  /**
 * @brief Encontra e reinvidica um bloco livre no disco.
 *
 * @return int Número do bloco livre ou -1 caso o disco esteja cheio ou não
 * montado.
 */
  int claim_free_block();
};

#endif
