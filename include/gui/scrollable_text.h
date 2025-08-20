#ifndef SCROLLABLE_TEXT_H
#define SCROLLABLE_TEXT_H

#include "graphic_interface.h"
#include "widget.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>

class GRAPHIC_INTERFACE::ScrollableText : public Widget {
private:
  sf::RectangleShape background;     // Visible area for the text
  sf::RectangleShape scrollbar;      // The scrollbar
  sf::RectangleShape scrollbarTrack; // The track for the scrollbar

  sf::View scrollView;     // View to manage scrolling
  sf::Text contentText;    // The text content
  std::string wrappedText; // Preprocessed text with line breaks
  float scrollOffset;      // Vertical scroll offset
  float scrollSpeed;       // Speed of scrolling
  float contentHeight;     // Height of the entire text content

  float scrollbarHeight; // Height of the scrollbar

  // Helper function to wrap text based on the width
  std::string wrapText(const std::string &text, const sf::Font &font,
                       unsigned int charSize, float maxWidth);

public:
  ScrollableText(float x, float y, float width, float height,
                 const sf::Font &font, const std::string &content,
                 unsigned int charSize = 16);

  void handleEvent(const sf::Event &event) override;

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif
