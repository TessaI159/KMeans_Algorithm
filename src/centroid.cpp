#include "centroid.h"
#include "pixel.h"

#include <cstdlib>

Centroid::Centroid() {};

Centroid::Centroid(std::vector<Pixel *> pixelVector_ptr)
{
  m_ownedPixels_ptr = pixelVector_ptr;
}

void Centroid::updateLocation()
{
  double rAverage{0};
  double gAverage{0};
  double bAverage{0};

  for(auto pixel : m_ownedPixels_ptr)
    {
      rAverage += pixel->r;
      gAverage += pixel->g;
      bAverage += pixel->b;
    }

  rAverage /= m_ownedPixels_ptr.size();
  gAverage /= m_ownedPixels_ptr.size();
  bAverage /= m_ownedPixels_ptr.size();

  m_location.r = rAverage;
  m_location.g = gAverage;
  m_location.b = bAverage;
  
  return;
}
