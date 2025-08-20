#ifndef IDODE_LIST_WIDGET_H
#define IDODE_LIST_WIDGET_H

#include "button.h"
#include "fs.h"
#include "graphic_interface.h"
#include "widget.h"
#include <SFML/Graphics/Drawable.hpp>

class GRAPHIC_INTERFACE::InodeList : public Widget {
private:
  sf::FloatRect box;
  std::vector<Button *> visible_inodes;
  std::vector<int> inodes;
  // Inode list sempre desenha um numero igual a
  // visible_inodes a partir de first_visible_indx
  std::size_t first_visible_indx = 0;
  float posX;
  float posY;
  float inodeItemSizeX;
  float inodeItemSizeY;
  float separator;
  std::function<void(int)> function;
  bool shouldCall;

  void updateInodeItens();
  void setShouldCall();

public:
  InodeList(float positionX, float positionY, std::size_t visible_inodes_qtt,
            sf::Font &font, sf::Color buttonTextColor, float itemSizeX = 300.0,
            float itemSizeY = 50.0, float separator = 20.0);
  ~InodeList();
  virtual void draw(sf::RenderTarget &target,
                    sf::RenderStates states) const override;
  void setInodes(std::vector<int> inodes);
  void setFunction(std::function<void(int)> function);
  void handleEvent(const sf::Event &event) override;
};

#endif
