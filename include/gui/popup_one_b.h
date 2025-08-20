#ifndef POPUP_ONE_B_H
#define POPUP_ONE_B_H

#include "graphic_interface.h"
#include "widget.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class GRAPHIC_INTERFACE::PopupOneB : public Widget {
private:
  sf::RectangleShape background;

  sf::RectangleShape inputBox;
  sf::Text inputText;
  std::string userInput;

  sf::Text instructionText;
  sf::Font font;

  bool isOpen;
  std::function<void(std::string)> function;

public:
  PopupOneB(float x, float y, float width, float height, const sf::Font &font);

  void open();
  void close();
  bool isActive() const;

  void handleEvent(const sf::Event &event) override;
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  void setFunction(std::function<void(std::string)> function);
};

#endif
