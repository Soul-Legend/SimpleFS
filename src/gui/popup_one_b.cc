#include "popup_one_b.h"
#include <SFML/Graphics.hpp>

using namespace GRAPHIC_INTERFACE;

PopupOneB::PopupOneB(float x, float y, float width, float height,
                     const sf::Font &font)
    : userInput(""), font(font), isOpen(false) {
  // Background
  background.setSize({ width, height });
  background.setPosition(x, y);
  background.setFillColor(sf::Color(50, 50, 50, 200)); // Semi-transparent

  inputBox.setSize({ width - 40, 40 });
  inputBox.setPosition(x + 20, y + 100);
  inputBox.setFillColor(sf::Color::White);
  inputBox.setOutlineThickness(2);
  inputBox.setOutlineColor(sf::Color::Black);

  inputText.setFont(font);
  inputText.setCharacterSize(16);
  inputText.setFillColor(sf::Color::Black);
  inputText.setPosition(x + 30, y + 110);

  instructionText.setFont(font);
  instructionText.setString("Caminho do arquivo");
  instructionText.setCharacterSize(16);
  instructionText.setFillColor(sf::Color::White);
  instructionText.setPosition(x + 20, y + 140);
}

void PopupOneB::open() {
  isOpen = true;
  userInput = "";
  inputText.setString(userInput);
}

void PopupOneB::close() {
  isOpen = false;
}

bool PopupOneB::isActive() const {
  return isOpen;
}

void PopupOneB::handleEvent(const sf::Event &event) {
  if (!isOpen)
    return;

  if (event.type == sf::Event::MouseButtonPressed &&
      event.mouseButton.button == sf::Mouse::Left) {
    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

    if (!background.getGlobalBounds().contains(mousePos)) {
      close();
    }
  } else if (event.type == sf::Event::TextEntered) {
    if (event.text.unicode >= 32 &&
        event.text.unicode <= 126) { // Printable characters
      userInput += static_cast<char>(event.text.unicode);
      inputText.setString(userInput);
    } else if (event.text.unicode == '\b' && !userInput.empty()) { // Backspace
      userInput.pop_back();
      inputText.setString(userInput);
    }
  } else if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Enter) {
      if (!userInput.empty()) {
        function(userInput);
        close();
      }
    } else if (event.key.code == sf::Keyboard::Escape) {
      close();
    }
  }
}

void PopupOneB::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  if (isOpen) {
    target.draw(background, states);

    target.draw(inputBox, states);
    target.draw(inputText, states);
    target.draw(instructionText, states);
  }
}

void PopupOneB::setFunction(std::function<void(std::string)> function) {
  this->function = function;
}
