#ifndef CENTROID_H
#define CENTROID_H

#include "pixel.h"
#include <vector>

class Centroid
{
private:
  int m_id{-1};
  std::vector<Pixel *> m_ownedPixels_ptr{};
  Pixel m_location{};
public:
  Centroid();
  Centroid(std::vector<Pixel *> pixelVector_ptr);
  int getID() { return m_id; }
  std::vector<Pixel *> getOwnedPixels_ptr() { return m_ownedPixels_ptr; }
  Pixel getLocation() { return m_location; }
  void updateLocation();
  double distanceFromPixel(Pixel *pixel_ptr);
  void addPixel(Pixel *pixel_ptr);
  Pixel* releasePixel(int pixelID);
};

#endif
