#include "block.h"

Block::Block() { m_colorNum = 1; }

bool Block::isActive() { return m_colorNum != 0; }

void Block::setColorNum(unsigned int colorNum) { m_colorNum = colorNum; }

Color Block::getColor() {
  // Conversion is handled by color header
  return colorPalette[m_colorNum];
}
