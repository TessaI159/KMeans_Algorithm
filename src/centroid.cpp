#include "centroid.h"
#include "pixel.h"

#include <cstdlib>
#include <math.h>

Centroid::Centroid() {};

Centroid::Centroid(std::vector<Pixel *> pixelVector_ptr)
{
  m_ownedPixels_ptr = pixelVector_ptr;
  updateLocation();
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

double Centroid::distanceFromPixel(Pixel *pixel_ptr)
{
  double pixel_r = static_cast<double>(pixel_ptr->r);
  double pixel_g = static_cast<double>(pixel_ptr->g);
  double pixel_b = static_cast<double>(pixel_ptr->b);
  double centroid_r = static_cast<double>(m_location.r);
  double centroid_g = static_cast<double>(m_location.g);
  double centroid_b = static_cast<double>(m_location.b);
  
  return (pow(centroid_r - pixel_r, 2) +
	  pow(centroid_g - pixel_g, 2) +
	  pow(centroid_b - pixel_b, 2));
}
