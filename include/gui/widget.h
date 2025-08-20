#ifndef WIDGET_H
#define WIDGET_H

#include "graphic_interface.h"
#include <SFML/Graphics.hpp>

class GRAPHIC_INTERFACE::Widget : public sf::Drawable {
public:
  virtual void handleEvent(const sf::Event &event) = 0;
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;
};

#endif
