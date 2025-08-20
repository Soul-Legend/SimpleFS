#include "inode_list.h"

using namespace GRAPHIC_INTERFACE;

InodeList::InodeList(float positionX, float positionY,
                     std::size_t visible_inodes_qtt, sf::Font& font, sf::Color buttonTextColor,
                     float itemSizeX, float itemSizeY, float separator)
    : posX(positionX), posY(positionY), inodeItemSizeX(itemSizeX),
      inodeItemSizeY(itemSizeY), separator(separator) {
  shouldCall = false;
  box = {posX, posY, inodeItemSizeX, inodeItemSizeY * visible_inodes.size() + separator * (visible_inodes.size() - 1)};
  for (std::size_t i = 0; i < visible_inodes_qtt; i++) {
    visible_inodes.emplace_back(new Button({ posX, posY + (inodeItemSizeY + separator) * i, inodeItemSizeX, inodeItemSizeY }, "", font, buttonTextColor));
    visible_inodes[i]->setFunction([this]() { this->setShouldCall(); });
  }
}

GRAPHIC_INTERFACE::InodeList::~InodeList() {
  for (Button *button : visible_inodes) {
    delete button;
  }
}

void InodeList::setInodes(std::vector<int> inodes) {
  this->inodes = inodes;
  updateInodeItens();
}

void GRAPHIC_INTERFACE::InodeList::setFunction(
    std::function<void(int)> function) {
  this->function = function;
}

void InodeList::handleEvent(const sf::Event &event) {
  for (std::size_t i = 0;
       i < visible_inodes.size() && first_visible_indx + i < inodes.size();
       ++i) {
    visible_inodes[i]->handleEvent(event);
    if (shouldCall) {
      function(inodes[first_visible_indx + i]);
      shouldCall = false;
      first_visible_indx = 0;
      updateInodeItens();
    }
  }
  sf::Vector2f mousePos(event.mouseWheelScroll.x, event.mouseWheelScroll.y);
  if (event.type == sf::Event::MouseWheelScrolled &&
      box.contains(mousePos.x, mousePos.y)) {
    if (event.mouseWheelScroll.delta < 0 &&
        first_visible_indx < inodes.size() - 1) {
      first_visible_indx++;
    } else if (event.mouseWheelScroll.delta > 0 && first_visible_indx > 0) {
      first_visible_indx--;
    }
    updateInodeItens();
  }
}

void InodeList::updateInodeItens() {
  // Só atualiza inodes visiveis
  for (std::size_t i = 0;
       i < visible_inodes.size() && first_visible_indx + i < inodes.size();
       i++) {
    visible_inodes[i]->setText("Inode " + std::to_string(inodes[first_visible_indx + i]));
  }
}

void InodeList::setShouldCall() {
  this->shouldCall = true;
}

void InodeList::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  // Só desenha inodes visiveis
  for (std::size_t i = 0;
       i < visible_inodes.size() && first_visible_indx + i < inodes.size();
       i++) {
    visible_inodes[i]->draw(target, states);
  }
}
