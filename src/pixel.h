#ifndef PIXEL_H
#define PIXEL_H


struct Pixel
{
  int r{0};
  int g{0};
  int b{0};
  int id{-1};
  int ownedBy{-1};
  int swapTo{-1};
  bool needsSwapped{false};
};

#endif
