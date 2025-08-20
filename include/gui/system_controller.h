#ifndef SYSTEM_CONTROLLER_H
#define SYSTEM_CONTROLLER_H

#include "disk.h"
#include "fs.h"
#include "file_ops.h"
#include "graphic_interface.h"
#include "gui_controller.h"
#include <SFML/Graphics.hpp>

using namespace GRAPHIC_INTERFACE;

class GRAPHIC_INTERFACE::SystemController {
private:
  typedef void (SystemController::*State)();
  GuiController gui_controller;

  std::string diskName;
  Disk *disk = nullptr;
  INE5412_FS fs;
  int selectedInode;

  bool openedDisk = false;

  // Telas do programa
  void changeToLastScreen();
  enum Screen {
    DISK_SELECT,
    DISK_CONTENT,
    INODE_CONTENT
  };

  // DiskSelectState
  void selectDisk(int nblocks, std::string diskPath);
  void formatDisk();
  void mountDisk();

  // DiskContentState
  void createInode();
  void setToInodeContentState(int inode_number);
  void setSuperblockContent(std::string superblock_content);
  std::string captureDoCopyout(int inumber, INE5412_FS *fs);

  // InodeContentState
  void setInodeDebugInfo(std::string inodeDebugInfo);
  void deleteSelectedInode();

public:
  SystemController();
  ~SystemController();
  void run();
};

#endif
