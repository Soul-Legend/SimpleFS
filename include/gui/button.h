#ifndef BUTTON_H
#define BUTTON_H

#include "graphic_interface.h"
#include "widget.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <functional>

class GRAPHIC_INTERFACE::Button : public Widget {
private:
  sf::FloatRect box;
  sf::Font btn_font;
  sf::Text txt;
  sf::RectangleShape btn;
  std::function<void()> function;
  void updateTextPos();
  void call();

public:
  Button(sf::FloatRect rect, const std::string &btn_txt, sf::Font& font, sf::Color& buttonTextColor);

  bool is_clicked(sf::Vector2f mousePos);

  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

  void handleEvent(const sf::Event &event) override;
  void setText(const std::string &btn_txt);
  void setPosition(float posX, float posY);
  void setFunction(std::function<void()> function);
  std::string getText();
};

#endif
