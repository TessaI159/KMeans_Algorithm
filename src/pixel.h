#ifndef PIXEL_H
#define PIXEL_H

#include <cstdint>

struct Pixel
{
  int_fast8_t r{0};
  int_fast8_t g{0};
  int_fast8_t b{0};
  int_fast32_t id{-1};
  int_fast16_t ownedBy{-1};
  int_fast16_t swapTo{-1};
  bool needsSwapped{false};
};

#endif
