#include "gui_controller.h"

using namespace GRAPHIC_INTERFACE;

GuiController::GuiController()
    : window(sf::VideoMode(960, 600), "SimpleFS",
             sf::Style::Titlebar | sf::Style::Close),
      buttonTextColor(sf::Color::White), normalTextColor(sf::Color::Black),
      messageTime(2.0), message("", font, 30), shouldShowMessage(false),
      selectedScreen(0),
      diskSelectionScreen(font, buttonTextColor, normalTextColor),
      diskContentScreen(font, buttonTextColor, normalTextColor),
      inodeContentScreen(font, buttonTextColor, normalTextColor) {
  window.setFramerateLimit(60);
  font.loadFromFile("fonts/FiraSans-Bold.otf");
  message.setFillColor(sf::Color::Green);
  message.setPosition(20.0f, 550.0f);
}

bool GuiController::isOpen() {
  return window.isOpen();
}

void GuiController::showMessage(std::string message) {
  this->message.setString(message);
  shouldShowMessage = true;
  clock.restart();
}

void GuiController::selectScreen(int screen) {
  selectedScreen = screen;
}

int GRAPHIC_INTERFACE::GuiController::getSelectedSceen() {
  return selectedScreen;
}

void GuiController::drawSelectedScreen() {
  window.clear(sf::Color::White);
  window.draw(*screens[selectedScreen]);
  if (shouldShowMessage) {
    if (clock.getElapsedTime().asSeconds() > messageTime) {
      shouldShowMessage = false;
      clock.restart();
    }
    window.draw(message);
  }
  window.display();
}

void GuiController::handleSelectedScreen() {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window.close();
    }
    screens[selectedScreen]->handleEvent(event);
  }
}

void GuiController::setDSPopupTwoBFunction(
    std::function<void(int, std::string)> function) {
  diskSelectionScreen.setPopupTwoBFunction(function);
}

void GuiController::setDSFormatFunction(std::function<void()> function) {
  diskSelectionScreen.setFormatFunction(function);
}

void GuiController::setDSMountFunction(std::function<void()> function) {
  diskSelectionScreen.setMountFunction(function);
}

void GuiController::setOpenedDisk() {
  diskSelectionScreen.setOpenedDisk();
}

void GuiController::setDCBackFunction(std::function<void()> function) {
  diskContentScreen.setBackFunc(function);
}

void GuiController::setDCCreateFunction(std::function<void()> function) {
  diskContentScreen.setCreateFunc(function);
}

void GuiController::setDiskName(std::string diskName) {
  diskContentScreen.setDiskName(diskName);
}

void GuiController::setSuperblockContent(std::string superblock_content) {
  diskContentScreen.setSuperBlockContent(superblock_content);
}

void GuiController::setInodeListContent(std::vector<int> inodes_inumber) {
  diskContentScreen.setInodes(inodes_inumber);
}

void GRAPHIC_INTERFACE::GuiController::setInodeListFunction(
    std::function<void(int)> function) {
  diskContentScreen.setInodeListFunction(function);
}

void GuiController::setICCopyInPopupFunction(
    std::function<void(std::string)> function) {
  inodeContentScreen.setCopyInPopupFunction(function);
}

void GuiController::setICCopyOutPopupFunction(
    std::function<void(std::string)> function) {
  inodeContentScreen.setCopyOutPopupFunction(function);
}

void GuiController::setICBackFunction(std::function<void()> function) {
  inodeContentScreen.setBackFunction(function);
}

void GuiController::setSelectedInode(int selectedInode) {
  inodeContentScreen.setSelectedInode(selectedInode);
}

void GuiController::setInodeContent(std::string inodeContent) {
  inodeContentScreen.setInodeContent(inodeContent);
}

void GuiController::setInodeDebugInfo(std::string inodeDebugInfo) {
  inodeContentScreen.setInodeDebugInfo(inodeDebugInfo);
}

void GuiController::setDeleteFunction(std::function<void()> function) {
  inodeContentScreen.setDeleteFunction(function);
}

void GuiController::setCopyInFunction(std::function<void()> function) {
  inodeContentScreen.setCopyInFunction(function);
}

void GuiController::setCopyOutFunction(std::function<void()> function) {
  inodeContentScreen.setCopyOutFunction(function);
}
