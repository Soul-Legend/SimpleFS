#include "disk_selection_screen.h"
#include <iostream>

using namespace GRAPHIC_INTERFACE;

DiskSelectionScreen::DiskSelectionScreen(sf::Font &font,
                                         sf::Color buttonTextColor,
                                         sf::Color normalTextColor)
    : font(font), buttonTextColor(buttonTextColor),
      normalTextColor(normalTextColor),
      selectButton({ initial_posX,
                     initial_posY + (button_sizeY + separator) * 0,
                     button_sizeX, button_sizeY },
                   "Selecionar Disco", font, buttonTextColor),
      formatButton({ initial_posX,
                     initial_posY + (button_sizeY + separator) * 1,
                     button_sizeX, button_sizeY },
                   "Formatar Disco", font, buttonTextColor),
      mountButton({ initial_posX, initial_posY + (button_sizeY + separator) * 2,
                    button_sizeX, button_sizeY },
                  "Montar Disco", font, buttonTextColor),
      selectionPopupTwoB(initial_posX, initial_posY, 400, 300, font) {

  openedDisk = false;
  setSelectFunction([this]() { this->openPopupTwoB(); });
}

void DiskSelectionScreen::openPopupTwoB() {
  selectionPopupTwoB.open();
}

void DiskSelectionScreen::handleEvent(const sf::Event &event) {
  selectButton.handleEvent(event);
  selectionPopupTwoB.handleEvent(event);
  if (openedDisk && !selectionPopupTwoB.isActive()) {
    formatButton.handleEvent(event);
    mountButton.handleEvent(event);
  }
}

void DiskSelectionScreen::draw(sf::RenderTarget &target,
                               sf::RenderStates states) const {
  selectButton.draw(target, states);
  if (openedDisk) {
    formatButton.draw(target, states);
    mountButton.draw(target, states);
  }
  selectionPopupTwoB.draw(target, states);
}

void DiskSelectionScreen::setSelectFunction(
    std::function<void()> function) {
  selectButton.setFunction(function);
}

void GRAPHIC_INTERFACE::DiskSelectionScreen::setPopupTwoBFunction(
    std::function<void(int, std::string)> function) {
  selectionPopupTwoB.setFunction(function);
}

void DiskSelectionScreen::setFormatFunction(std::function<void()> function) {
  formatButton.setFunction(function);
}

void DiskSelectionScreen::setMountFunction(
    std::function<void()> function) {
  mountButton.setFunction(function);
}

void DiskSelectionScreen::setOpenedDisk() {
  openedDisk = true;
}

