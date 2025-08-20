#ifndef INODE_CONTENT_SCREEN_H
#define INODE_CONTENT_SCREEN_H

#include "button.h"
#include "graphic_interface.h"
#include "popup_one_b.h"
#include "scrollable_text.h"
#include "widget.h"
#include <SFML/Graphics.hpp>

using namespace GRAPHIC_INTERFACE;

class GRAPHIC_INTERFACE::InodeContentScreen : public Widget {
private:
  sf::Font font;
  sf::Color buttonTextColor;
  sf::Color normalTextColor;
  float button_sizeX = 200;
  float button_sizeY = 50;
  float initial_posX = 0;
  float initial_posY = 0;
  float title_line = initial_posY + 100;
  float screen_division = 490;
  float scrollableTextSizeX = 450;
  float scrollableTextSizeY = 330;

  sf::Text selectedInode;
  sf::Text inodeDebugTitle;
  sf::Text catTitle;
  Button backButton;
  Button deleteButton;
  Button copyInButton;
  Button copyOutButton;
  ScrollableText inodeDebugInfo;
  ScrollableText inodeContent;
  PopupOneB copyInPopup;
  PopupOneB copyOutPopup;

public:
  InodeContentScreen(sf::Font &font, sf::Color buttonTextColor,
                     sf::Color normalTextColor);

  void openCopyInPopup();
  void setCopyInPopupFunction(std::function<void(std::string)> function);

  void openCopyOutPopup();
  void setCopyOutPopupFunction(std::function<void(std::string)> function);

  void handleEvent(const sf::Event &event) override;
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  void setSelectedInode(int selectedInode);
  void setInodeContent(std::string inodeContent);
  void setInodeDebugInfo(std::string inodeDebugInfo);
  void setBackFunction(std::function<void()> function);
  void setDeleteFunction(std::function<void()> function);
  void setCopyInFunction(std::function<void()> function);
  void setCopyOutFunction(std::function<void()> function);
};

#endif
