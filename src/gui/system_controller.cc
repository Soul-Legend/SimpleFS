#include "system_controller.h"
#include "file_ops.h"
#include "fs.h"
#include "graphic_interface.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace GRAPHIC_INTERFACE;

SystemController::SystemController()
    : gui_controller(GuiController()), fs(disk) {
  gui_controller.selectScreen(DISK_SELECT);

  // Disk Selection State
  gui_controller.setDSPopupTwoBFunction([this](int nblocks,
                                               std::string diskPath) {
    if (nblocks < 3) {
      gui_controller.showMessage("Choose a number of blocks greater than two!");
      return;
    }
    this->selectDisk(nblocks, diskPath);
  });
  gui_controller.setDSFormatFunction([this]() { this->formatDisk(); });
  gui_controller.setDSMountFunction([this]() { this->mountDisk(); });

  // Disk Content State
  gui_controller.setDCBackFunction([this]() { this->changeToLastScreen(); });
  gui_controller.setDCCreateFunction([this]() { this->createInode(); });
  gui_controller.setInodeListFunction(
      [this](int inode) { this->setToInodeContentState(inode); });

  // Inode Content State
  gui_controller.setICCopyInPopupFunction([this](std::string filePath) {
    if (!File_Ops::do_copyin(filePath.c_str(), selectedInode, &fs)) {
      gui_controller.showMessage("Insert a valid filename!");
    } else {
      // Atualiza a tela com o conteúdo copiado.
      gui_controller.setInodeContent(captureDoCopyout(selectedInode, &fs));
      gui_controller.setInodeDebugInfo(fs.get_inode_info(selectedInode));

      std::string disk_full_msg =
          fs.fs_is_disk_full() ? "Disk has become full | " : "";

      gui_controller.showMessage(disk_full_msg + "File " + filePath +
                                " copied in to inode " +
                                std::to_string(selectedInode));
    }
  });
  gui_controller.setICCopyOutPopupFunction([this](std::string filePath) {
    if (!File_Ops::do_copyout(selectedInode, filePath.c_str(), &fs)) {
      gui_controller.showMessage("Insert a valid filename!");
    } else {
      gui_controller.showMessage("Inode " + std::to_string(selectedInode) +
                                " copied out to " + filePath);
    }
  });

  gui_controller.setICBackFunction([this]() { this->changeToLastScreen(); });
  gui_controller.setDeleteFunction([this]() { this->deleteSelectedInode(); });
}

SystemController::~SystemController() {
  delete disk;
}

void SystemController::run() {
  while (gui_controller.isOpen()) {
    gui_controller.handleSelectedScreen();
    gui_controller.drawSelectedScreen();
  }
}

/**
 * @brief Retorna a tela anterior.
 */
void SystemController::changeToLastScreen() {
  gui_controller.selectScreen(gui_controller.getSelectedSceen() - 1);
}

void SystemController::selectDisk(int nblocks, std::string diskPath) {
  diskName = std::filesystem::path(diskPath).filename();
  Disk * diskTemp = new Disk(diskPath.c_str(), nblocks);
  // Caso não tenha sido possível abrir o arquivo, deleta e informa ao usuário
  if (diskTemp->size() == -1) {
    if (!openedDisk) {
      delete diskTemp;
    }
    gui_controller.showMessage("Insert a valid filename!");
  } else {
    // Caso já exista um disco aberto, deleta e utiliza o novo disco
    if (openedDisk) {
      delete disk;
    }
    disk = diskTemp;
    fs = INE5412_FS(disk);
    openedDisk = true;
    gui_controller.setOpenedDisk();
  }
}

void SystemController::setSuperblockContent(std::string superblock_content) {
  gui_controller.setSuperblockContent(superblock_content);
}

void SystemController::formatDisk() {
  if (!openedDisk) {
    return;
  }
  if (!fs.getIsMounted()) {
    fs.fs_format();
    gui_controller.showMessage("The disk has been successfully formatted!");
  } else {
    gui_controller.showMessage("Disk already mounted!");
  }
}

void SystemController::mountDisk() {
  if (fs.fs_mount()) {
    gui_controller.setSuperblockContent(fs.get_superblock_info());
    gui_controller.setInodeListContent(fs.get_inode_list());
    gui_controller.setDiskName(diskName);
    gui_controller.selectScreen(DISK_CONTENT);
    gui_controller.showMessage("The disk has been successfully mounted!");
  } else if (fs.getIsMounted()) {
    gui_controller.selectScreen(DISK_CONTENT);
    gui_controller.showMessage(
        "Disk already mounted, redirecting to the next screen!");
  } else {
    gui_controller.showMessage("Disk not formatted!");
  }
}

// DiskContentState

void SystemController::createInode() {
  if (fs.fs_create()) {
    gui_controller.setInodeListContent(fs.get_inode_list());
    gui_controller.showMessage("A new inode has been created!");
  } else {
    gui_controller.showMessage("No inodes left");
  }
}

void SystemController::setToInodeContentState(int inode_number) {
  selectedInode = inode_number;
  gui_controller.setSelectedInode(inode_number);
  gui_controller.setInodeContent(captureDoCopyout(inode_number, &fs));
  gui_controller.setInodeDebugInfo(fs.get_inode_info(inode_number));
  gui_controller.selectScreen(INODE_CONTENT);
}

// InodeContentState

void SystemController::setInodeDebugInfo(std::string inodeDebugInfo) {
  gui_controller.setInodeDebugInfo(inodeDebugInfo);
}

void SystemController::deleteSelectedInode() {
  fs.fs_delete(selectedInode);
  gui_controller.setInodeListContent(fs.get_inode_list());
  gui_controller.showMessage("Inode has been successfully deleted!");
  changeToLastScreen();
}

std::string SystemController::captureDoCopyout(int inumber, INE5412_FS *fs) {
  // Cria um arquivo temporário
  char tempFilename[] = "/tmp/copyoutXXXXXX";
  int fd = mkstemp(tempFilename);
  if (fd == -1) {
    throw std::runtime_error("Falha ao criar arquivo temporário");
  }
  close(fd);

  // Caso copyOut falhe
  if (!File_Ops::do_copyout(inumber, tempFilename, fs)) {
    unlink(tempFilename);
    throw std::runtime_error("do_copyout falhou");
  }

  // Lê o conteúdo de volta para uma string
  std::ifstream tempFile(tempFilename, std::ios::in | std::ios::binary);
  std::string content((std::istreambuf_iterator<char>(tempFile)),
                      std::istreambuf_iterator<char>());

  // Deleta arquivo temporário
  tempFile.close();
  unlink(tempFilename);

  return content;
}
