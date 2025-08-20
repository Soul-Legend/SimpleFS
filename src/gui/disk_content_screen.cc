#include "disk_content_screen.h"
#include "graphic_interface.h"

using namespace GRAPHIC_INTERFACE;

DiskContentScreen::DiskContentScreen(sf::Font &font, sf::Color &buttonTextColor,
                                     sf::Color &normalTextColor)
    : font(font), buttonTextColor(buttonTextColor),
      normalTextColor(normalTextColor), diskName("Disk Name", font, 30),
      superBlockTitle("Superblock", font, 30),
      inodeListTitle("Inode List", font, 30),
      createButton(
          { initial_posX + 600, buttonPosY, button_sizeX, button_sizeY },
          "Create", font, buttonTextColor),
      backButton({ initial_posX + 50, buttonPosY, button_sizeX, button_sizeY },
                 "Back", font, buttonTextColor),
      inodeList(inodeListWidgetPosX, initial_posY + 100, 5, font,
                buttonTextColor),
      superBlockInfo(initial_posX, initial_posY + 100, scrollableTextSizeX,
                     scrollableTextSizeY, font, "") {

  diskName.setPosition(initial_posX, initial_posY);
  diskName.setFillColor(normalTextColor);

  // Widget Superblock

  superBlockTitle.setPosition(initial_posX + 100, initial_posY + 50);
  superBlockTitle.setFillColor(normalTextColor);

  inodeListTitle.setPosition(inodeListWidgetPosX + 80, initial_posY + 50);
  inodeListTitle.setFillColor(normalTextColor);
}

void DiskContentScreen::handleEvent(const sf::Event &event) {
  createButton.handleEvent(event);
  backButton.handleEvent(event);
  inodeList.handleEvent(event);
  superBlockInfo.handleEvent(event);
}

void DiskContentScreen::draw(sf::RenderTarget &target,
                             sf::RenderStates states) const {
  target.draw(diskName, states);
  target.draw(superBlockTitle, states);
  target.draw(inodeListTitle, states);
  createButton.draw(target, states);
  backButton.draw(target, states);
  inodeList.draw(target, states);
  superBlockInfo.draw(target, states);
}

void DiskContentScreen::setInodes(std::vector<int> inodes) {
  inodeList.setInodes(inodes);
}

void DiskContentScreen::setSuperBlockContent(
    std::string superblockContent) {
  font.loadFromFile("fonts/FiraSans-Bold.otf");
  superBlockInfo =
      ScrollableText(initial_posX, initial_posY + 100, scrollableTextSizeX,
                     scrollableTextSizeY, font, superblockContent);
}

void GRAPHIC_INTERFACE::DiskContentScreen::setInodeListFunction(
    std::function<void(int)> function) {
  inodeList.setFunction(function);
}

void DiskContentScreen::setBackFunc(
    std::function<void()> function) {
  backButton.setFunction(function);
}

void DiskContentScreen::setCreateFunc(
    std::function<void()> function) {
  createButton.setFunction(function);
}

void DiskContentScreen::setDiskName(std::string name) {
  diskName.setString(name);
}
