#ifndef DISK_SELECTION_SCREEN_H
#define DISK_SELECTION_SCREEN_H

#include "button.h"
#include "widget.h"
#include "graphic_interface.h"
#include "popup_two_b.h"
#include <SFML/Graphics.hpp>

using namespace GRAPHIC_INTERFACE;

class GRAPHIC_INTERFACE::DiskSelectionScreen : public Widget {
private:
  sf::Font font;
  sf::Color buttonTextColor;
  sf::Color normalTextColor;
  float button_sizeX = 400;
  float button_sizeY = 50;
  float initial_posX = 480 - button_sizeX / 2;
  float initial_posY = 100;
  float separator = 20;
  bool openedDisk;
  Button selectButton;
  Button formatButton;
  Button mountButton;
  void openPopupTwoB();
  void setSelectFunction(std::function<void()> function);

public:

  PopupTwoB selectionPopupTwoB;

  DiskSelectionScreen(sf::Font &font, sf::Color buttonTextColor,
                      sf::Color normalTextColor);
  void handleEvent(const sf::Event &event) override;
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  void setPopupTwoBFunction(std::function<void(int, std::string)> function);
  void setFormatFunction(std::function<void()> function);
  void setMountFunction(std::function<void()> function);

  void setOpenedDisk();
};

#endif
