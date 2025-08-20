#ifndef DISK_CONTENT_SCREEN_H
#define DISK_CONTENT_SCREEN_H

#include "button.h"
#include "widget.h"
#include "graphic_interface.h"
#include "inode_list.h"
#include "scrollable_text.h"
#include <SFML/Graphics.hpp>

using namespace GRAPHIC_INTERFACE;

class GRAPHIC_INTERFACE::DiskContentScreen : public Widget {
private:
  sf::Font font;
  sf::Color buttonTextColor;
  sf::Color normalTextColor;
  float initial_posX = 50;
  float initial_posY = 50;
  float button_sizeX = 200;
  float button_sizeY = 50;
  float separator = 20;
  float buttonPosY = initial_posY + 450;
  float scrollableTextSizeX = 450;
  float scrollableTextSizeY = 330;
  float inodeListWidgetPosX = initial_posX + 550;

public:
  sf::Text diskName;
  sf::Text superBlockTitle;
  sf::Text inodeListTitle;
  Button createButton;
  Button backButton;
  InodeList inodeList;
  ScrollableText superBlockInfo;

  void setBackFunc(std::function<void()> function);
  void setCreateFunc(std::function<void()> function);

  void setDiskName(std::string name);
  void setInodes(std::vector<int> inodes);
  void setSuperBlockContent(std::string superblockContent);
  void setInodeListFunction(std::function<void(int)> function);

  DiskContentScreen(sf::Font &font, sf::Color &buttonTextColor,
                    sf::Color &normalTextColor);
  void handleEvent(const sf::Event &event) override;
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif
