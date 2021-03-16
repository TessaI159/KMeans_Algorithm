#ifndef PIXEL_H
#define PIXEL_H

// A simple structure with an r, g, b, id and int to show who they are owned by
// All pixels must have a unique ID
struct Pixel
{
  int r{0};
  int g{0};
  int b{0};
  int id{-1};
  int ownedBy{-1};
};

#endif
