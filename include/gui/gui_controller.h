#ifndef GUI_CONTROLLER_H
#define GUI_CONTROLLER_H

#include "disk_content_screen.h"
#include "disk_selection_screen.h"
#include "graphic_interface.h"
#include "inode_content_screen.h"
#include <SFML/Graphics.hpp>

using namespace GRAPHIC_INTERFACE;

class GRAPHIC_INTERFACE::GuiController {
private:
  sf::RenderWindow window;
  sf::Font font;
  sf::Color buttonTextColor;
  sf::Color normalTextColor;

  sf::Clock clock;
  float messageTime;
  sf::Text message;
  bool shouldShowMessage;
  int selectedScreen;

  DiskSelectionScreen diskSelectionScreen;
  DiskContentScreen diskContentScreen;
  InodeContentScreen inodeContentScreen;

  Widget *screens[3] = { &diskSelectionScreen, &diskContentScreen,
                         &inodeContentScreen };

public:
  GuiController();

  bool isOpen();

  void showMessage(std::string message);

  void selectScreen(int screen);
  int getSelectedSceen();
  void drawSelectedScreen();
  void handleSelectedScreen();

  // DiskSelectionScreen
  void setDSPopupTwoBFunction(std::function<void(int, std::string)> function);
  void setDSFormatFunction(std::function<void()> function);
  void setDSMountFunction(std::function<void()> function);
  void setOpenedDisk();

  // DiskContentScreen
  void setDCBackFunction(std::function<void()> function);
  void setDCCreateFunction(std::function<void()> function);
  void setDiskName(std::string diskName);
  void setSuperblockContent(std::string superblock_content);
  void setInodeListContent(std::vector<int> inodes_inumber);
  void setInodeListFunction(std::function<void(int)> function);

  // InodeContentState
  void setICCopyInPopupFunction(std::function<void(std::string)> function);
  void setICCopyOutPopupFunction(std::function<void(std::string)> function);
  void setICBackFunction(std::function<void()> function);
  void setSelectedInode(int selectedInode);
  void setInodeContent(std::string inodeContent);
  void setInodeDebugInfo(std::string inodeDebugInfo);
  void setDeleteFunction(std::function<void()> function);
  void setCopyInFunction(std::function<void()> function);
  void setCopyOutFunction(std::function<void()> function);

  void getCopyInInfo();
  void getCopyOutInfo();
};

#endif
