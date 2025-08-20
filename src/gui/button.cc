#include "button.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <string>

using namespace GRAPHIC_INTERFACE;

Button::Button(sf::FloatRect rect, const std::string &btn_txt, sf::Font &font,
               sf::Color &buttonTextColor) {
  box = rect;

  btn.setSize({ box.width, box.height });
  btn.setFillColor(sf::Color(40, 40, 40));
  btn.setPosition(box.left, box.top);

  txt = sf::Text(btn_txt, font, 30);
  updateTextPos();
  txt.setFillColor(buttonTextColor);
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(btn, states);
  target.draw(txt, states);
}

bool Button::is_clicked(sf::Vector2f mousePos) {
  return box.contains(mousePos);
}

void Button::handleEvent(const sf::Event &event) {
  if (event.type == sf::Event::MouseButtonPressed &&
      event.mouseButton.button == sf::Mouse::Left) {
    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
    if (is_clicked(mousePos)) {
      call();
    }
  }
}

void Button::updateTextPos() {
  float width = 0;
  for (size_t i = 0; i < txt.getString().getSize(); ++i) {
    width += 14;
  }
  sf::FloatRect btnRect = btn.getGlobalBounds();
  txt.setPosition(btnRect.left + (btnRect.width - width)/2, btnRect.top);
}

void Button::call() {
  function();
}

void Button::setText(const std::string &btn_txt) {
  this->txt.setString(btn_txt);
  updateTextPos();
}

void Button::setPosition(float posX, float posY) {
  this->btn.setPosition(posX, posY);
  updateTextPos();
}

void GRAPHIC_INTERFACE::Button::setFunction(std::function<void()> func) {
  function = func;
}

std::string Button::getText() {
  return this->txt.getString();
}
