#include "popup_two_b.h"
#include <SFML/Graphics.hpp>

using namespace GRAPHIC_INTERFACE;

PopupTwoB::PopupTwoB(float x, float y, float width, float height, const sf::Font &font)
    : userInput1(""), userInput2(""), font(font), activeInput(1),
      isOpen(false) {
  // Background
  background.setSize({ width, height });
  background.setPosition(x, y);
  background.setFillColor(sf::Color(50, 50, 50, 200)); // Semi-transparent

  // First input box
  inputBox1.setSize({ width - 40, 40 });
  inputBox1.setPosition(x + 20, y + 100);
  inputBox1.setFillColor(sf::Color::White);
  inputBox1.setOutlineThickness(2);
  inputBox1.setOutlineColor(sf::Color::Black);

  inputText1.setFont(font);
  inputText1.setCharacterSize(16);
  inputText1.setFillColor(sf::Color::Black);
  inputText1.setPosition(x + 30, y + 110);

  instructionText1.setFont(font);
  instructionText1.setString("Quantidade de blocos:");
  instructionText1.setCharacterSize(16);
  instructionText1.setFillColor(sf::Color::White);
  instructionText1.setPosition(x + 20, y + 70);

  // Second input box
  inputBox2.setSize({ width - 40, 40 });
  inputBox2.setPosition(x + 20, y + 160);
  inputBox2.setFillColor(sf::Color::White);
  inputBox2.setOutlineThickness(2);
  inputBox2.setOutlineColor(sf::Color::Black);

  inputText2.setFont(font);
  inputText2.setCharacterSize(16);
  inputText2.setFillColor(sf::Color::Black);
  inputText2.setPosition(x + 30, y + 170);

  instructionText2.setFont(font);
  instructionText2.setString("Caminho da imagem:");
  instructionText2.setCharacterSize(16);
  instructionText2.setFillColor(sf::Color::White);
  instructionText2.setPosition(x + 20, y + 140);
}

void PopupTwoB::open() {
  isOpen = true;
  userInput1 = "";
  userInput2 = "";
  inputText1.setString(userInput1);
  inputText2.setString(userInput2);
}

void PopupTwoB::close() {
  isOpen = false;
}

bool PopupTwoB::isActive() const {
  return isOpen;
}

void PopupTwoB::handleEvent(
    const sf::Event &event) {
  if (!isOpen)
    return;

  if (event.type == sf::Event::MouseButtonPressed &&
      event.mouseButton.button == sf::Mouse::Left) {
    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

    if (inputBox1.getGlobalBounds().contains(mousePos)) {
      activeInput = 1;
    } else if (inputBox2.getGlobalBounds().contains(mousePos)) {
      activeInput = 2;
    } else if (!background.getGlobalBounds().contains(mousePos)) {
      close();
    }
  } else if (event.type == sf::Event::TextEntered) {
    if (activeInput == 1) {
      if (event.text.unicode >= '0' && event.text.unicode <= '9') {
        userInput1 += static_cast<char>(event.text.unicode);
        inputText1.setString(userInput1);
      } else if (event.text.unicode == '\b' &&
                 !userInput1.empty()) { // Backspace
        userInput1.pop_back();
        inputText1.setString(userInput1);
      }
    } else if (activeInput == 2) {
      if (event.text.unicode >= 32 &&
          event.text.unicode <= 126) { // Printable characters
        userInput2 += static_cast<char>(event.text.unicode);
        inputText2.setString(userInput2);
      } else if (event.text.unicode == '\b' &&
                 !userInput2.empty()) { // Backspace
        userInput2.pop_back();
        inputText2.setString(userInput2);
      }
    }
  } else if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Enter) {
      if (!userInput1.empty()) {
        function(std::stoi(userInput1), userInput2);
        close();
      }
    } else if (event.key.code == sf::Keyboard::Escape) {
      close();
    } else if (event.key.code == sf::Keyboard::Tab) {
      activeInput = (activeInput == 1) ? 2 : 1;
    }
  }
}

void PopupTwoB::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  if (isOpen) {
    target.draw(background, states);

    target.draw(inputBox1, states);
    target.draw(inputText1, states);
    target.draw(instructionText1, states);

    target.draw(inputBox2, states);
    target.draw(inputText2, states);
    target.draw(instructionText2, states);
  }
}

void PopupTwoB::setFunction(
    std::function<void(int, std::string)> function) {
  this->function = function;
}
