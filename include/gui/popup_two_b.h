#ifndef POPUP_TWO_B_H
#define POPUP_TWO_B_H

#include "graphic_interface.h"
#include "widget.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class GRAPHIC_INTERFACE::PopupTwoB : public Widget {
private:
  sf::RectangleShape background;

  // First input box (integer)
  sf::RectangleShape inputBox1;
  sf::Text inputText1;
  std::string userInput1;

  // Second input box (generic text)
  sf::RectangleShape inputBox2;
  sf::Text inputText2;
  std::string userInput2;

  sf::Text instructionText1;
  sf::Text instructionText2;
  sf::Font font;

  int activeInput; // 1 = first box, 2 = second box
  bool isOpen;
  std::function<void(int, std::string)> function;

public:
  PopupTwoB(float x, float y, float width, float height, const sf::Font &font);

  void open();
  void close();
  bool isActive() const;

  void handleEvent(const sf::Event &event) override;
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  void setFunction(std::function<void(int, std::string)> function);
};

#endif
