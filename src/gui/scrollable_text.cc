#include "scrollable_text.h"
#include "graphic_interface.h"
#include <iostream>
#include <sstream>

using namespace GRAPHIC_INTERFACE;

// Helper function to wrap text based on the width
std::string ScrollableText::wrapText(const std::string &text,
                                     const sf::Font &font,
                                     unsigned int charSize, float maxWidth) {
  std::stringstream wrapped;
  std::string word;
  std::istringstream stream(text);
  float spaceWidth = font.getGlyph(' ', charSize, false).advance;
  float lineWidth = 0;

  while (stream >> word) {
    float wordWidth = 0;
    std::string wordTemp;
    for (std::size_t i = 0; i < word.size(); i++) {
      wordWidth += 9;
      wordTemp += word[i];
      if (wordWidth > maxWidth) {
        wrapped << wordTemp;
        wordTemp = "";
        wrapped << "\n";
        wordWidth = 0;
        lineWidth = 0;
      }
    }

    if (lineWidth + wordWidth + spaceWidth > maxWidth) {
      wrapped << "\n";
      lineWidth = 0;
    } else if (lineWidth > 0) {
      wrapped << " ";
      lineWidth += spaceWidth;
    }

    wrapped << wordTemp;
    wordTemp = "";
    lineWidth += wordWidth;

    if (stream.peek() == '\n') {
      wrapped << '\n';
      lineWidth = 0;
    }
  }

  return wrapped.str();
}

ScrollableText::ScrollableText(float x, float y, float width, float height,
                               const sf::Font &font, const std::string &content,
                               unsigned int charSize)
    : scrollOffset(0), scrollSpeed(30.0f) {

  // Background (acts as the visible area for text)
  background.setSize({ width, height });
  background.setPosition(x, y);
  background.setFillColor(sf::Color::White);
  background.setOutlineThickness(2);
  background.setOutlineColor(sf::Color::Black);

  // Wrap text to fit within the visible area
  wrappedText = wrapText(content, font, charSize, 0.8 * width - 20);

  // Text content
  contentText.setFont(font);
  contentText.setString(wrappedText);
  contentText.setCharacterSize(charSize);
  contentText.setFillColor(sf::Color::Black);
  contentText.setPosition(10,
                          5); // Slight padding inside the background

  // Calculate content height
  // sf::FloatRect textBounds = contentText.getLocalBounds();
  // contentHeight = textBounds.height + 10; // Add padding to content height
  // Calculate content height based on the number of '\n' in wrappedText
  int numLines = std::count(wrappedText.begin(), wrappedText.end(), '\n') + 1;
  contentHeight = numLines * 20;

  // View for scrolling
  scrollView.reset(sf::FloatRect(0, 0, width, height));
  scrollView.setViewport(
      sf::FloatRect(x / 960, y / 600, width / 960, height / 600));

  // Scrollbar track
  scrollbarTrack.setSize({ 10, height });
  scrollbarTrack.setPosition(x + width - 15, y);
  scrollbarTrack.setFillColor(sf::Color(200, 200, 200));

  // Scrollbar
  scrollbarHeight = std::max(std::min(height * (height / contentHeight), height),
                             20.0f); // Minimum height of scrollbar
  scrollbar.setSize({ 10, scrollbarHeight });
  scrollbar.setPosition(x + width - 15, y);
  scrollbar.setFillColor(sf::Color(100, 100, 100));
}

void ScrollableText::handleEvent(const sf::Event &event) {
  if (event.type == sf::Event::MouseWheelScrolled &&
      event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel && 
      background.getGlobalBounds().contains(static_cast<float>(event.mouseWheelScroll.x), static_cast<float>(event.mouseWheelScroll.y))) {
      float delta = event.mouseWheelScroll.delta;
      scrollOffset -= delta * scrollSpeed;

      // Clamp scroll offset
      scrollOffset = std::max(
          0.0f, std::min(scrollOffset, contentHeight - background.getSize().y));

      // Update scrollbar position
      float maxScroll = contentHeight - background.getSize().y;
      float scrollRatio = scrollOffset / maxScroll;
      scrollbar.setPosition(scrollbar.getPosition().x,
                            background.getPosition().y +
                                scrollRatio *
                                    (background.getSize().y - scrollbarHeight));

      // Set view to show a part of the content
      scrollView.reset(sf::FloatRect(0, scrollOffset, background.getSize().x,
                                     background.getSize().y));
  }
}

void ScrollableText::draw(sf::RenderTarget &target,
                          sf::RenderStates states) const {
  // Draw the background and scrollbar
  target.draw(background, states);

  target.setView(scrollView);

  // Draw content within the scroll view
  target.draw(contentText, states);

  // Reset view to default
  target.setView(target.getDefaultView());

  target.draw(scrollbarTrack, states);
  target.draw(scrollbar, states);
}
