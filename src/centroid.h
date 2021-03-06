#ifndef CENTROID_H
#define CENTROID_H

#include "pixel.h"

#include <vector>
#include <fstream>

class Centroid
{
private:
  int m_id{-1};
  std::vector<Pixel *> m_ownedPixels_ptr{};
  Pixel m_location{};
  
public:
  Centroid();
  Centroid(std::vector<Pixel *> pixelVector_ptr);
  int getID();
  void setID(int id);
  Pixel getLocation();
  void updateLocation();
  double distanceFromPixel(Pixel *pixel_ptr);
  void addPixel(Pixel *pixel_ptr);
  Pixel* releasePixel(int pixelID);
  void printLocation(bool newLine, std::ofstream& output);
  double distortion();
};

#endif
