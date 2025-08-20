#include "inode_content_screen.h"
#include <iostream>
using namespace GRAPHIC_INTERFACE;

InodeContentScreen::InodeContentScreen(sf::Font &font,
                                       sf::Color buttonTextColor,
                                       sf::Color normalTextColor)
    : font(font), buttonTextColor(buttonTextColor),
      normalTextColor(normalTextColor), selectedInode("", font, 30),
      inodeDebugTitle("Debug information", font, 30),
      catTitle("Inode content", font, 30),
      backButton(
          { initial_posX + 30, initial_posY + 500, button_sizeX, button_sizeY },
          "Back", font, buttonTextColor),
      deleteButton({ initial_posX + button_sizeX + 60, initial_posY + 500,
                     button_sizeX, button_sizeY },
                   "Delete", font, buttonTextColor),
      copyInButton({ initial_posX + screen_division + 10, initial_posY + 500,
                     button_sizeX, button_sizeY },
                   "CopyIn", font, buttonTextColor),
      copyOutButton({ initial_posX + screen_division + button_sizeX + 40,
                      initial_posY + 500, button_sizeX, button_sizeY },
                    "CopyOut", font, buttonTextColor),
      inodeDebugInfo(initial_posX + 20, initial_posY + title_line + 50,
                     scrollableTextSizeX, scrollableTextSizeY, font, ""),
      inodeContent(initial_posX + screen_division,
                   initial_posY + title_line + 50, scrollableTextSizeX,
                   scrollableTextSizeY, font, ""),
      copyInPopup(280, 175, 400, 250, font),
      copyOutPopup(280, 175, 400, 250, font) {

  selectedInode.setPosition(initial_posX + 50, initial_posY + 50);
  selectedInode.setFillColor(normalTextColor);
  inodeDebugTitle.setPosition(initial_posX + 100, title_line);
  inodeDebugTitle.setFillColor(normalTextColor);
  catTitle.setPosition(initial_posX + screen_division + 120, title_line);
  catTitle.setFillColor(normalTextColor);

  setCopyInFunction([this]() { this->openCopyInPopup(); });
  setCopyOutFunction([this]() { this->openCopyOutPopup(); });
}

void InodeContentScreen::openCopyInPopup() {
  copyInPopup.open();
}

void InodeContentScreen::setCopyInPopupFunction(
    std::function<void(std::string)> function) {
  copyInPopup.setFunction(function);
}

void InodeContentScreen::openCopyOutPopup() {
  copyOutPopup.open();
}

void InodeContentScreen::setCopyOutPopupFunction(
    std::function<void(std::string)> function) {
  copyOutPopup.setFunction(function);
}

void InodeContentScreen::handleEvent(const sf::Event &event) {
  backButton.handleEvent(event);
  copyInPopup.handleEvent(event);
  copyOutPopup.handleEvent(event);
  if (!copyInPopup.isActive() || !copyOutPopup.isActive()) {
    deleteButton.handleEvent(event);
    copyInButton.handleEvent(event);
    copyOutButton.handleEvent(event);
  }
  inodeDebugInfo.handleEvent(event);
  inodeContent.handleEvent(event);
}

void InodeContentScreen::draw(sf::RenderTarget &target,
                              sf::RenderStates states) const {
  target.draw(selectedInode, states);
  target.draw(inodeDebugTitle, states);
  target.draw(catTitle, states);
  backButton.draw(target, states);
  deleteButton.draw(target, states);
  copyInButton.draw(target, states);
  copyOutButton.draw(target, states);
  inodeDebugInfo.draw(target, states);
  inodeContent.draw(target, states);
  copyInPopup.draw(target, states);
  copyOutPopup.draw(target, states);
}

void InodeContentScreen::setInodeDebugInfo(std::string inodeDebugInfo) {
  this->inodeDebugInfo = ScrollableText(
      initial_posX + 20, initial_posY + title_line + 50, scrollableTextSizeX,
      scrollableTextSizeY, font, inodeDebugInfo);
}

void InodeContentScreen::setBackFunction(std::function<void()> function) {
  backButton.setFunction(function);
}

void InodeContentScreen::setDeleteFunction(std::function<void()> function) {
  deleteButton.setFunction(function);
}

void InodeContentScreen::setCopyInFunction(std::function<void()> function) {
  copyInButton.setFunction(function);
}

void InodeContentScreen::setCopyOutFunction(std::function<void()> function) {
  copyOutButton.setFunction(function);
}

void GRAPHIC_INTERFACE::InodeContentScreen::setSelectedInode(
    int selectedInode) {
  this->selectedInode.setString("Inode " + std::to_string(selectedInode));
}

void InodeContentScreen::setInodeContent(std::string inodeContent) {
  font.loadFromFile("fonts/FiraSans-Bold.otf");
  this->inodeContent = ScrollableText(
      initial_posX + screen_division, initial_posY + title_line + 50,
      scrollableTextSizeX, scrollableTextSizeY, font, inodeContent);
}
