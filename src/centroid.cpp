#include "centroid.h"
#include "pixel.h"

#include <math.h>

Centroid::Centroid() {};

Centroid::Centroid(std::vector<Pixel *> pixelVector_ptr)
{
  m_ownedPixels_ptr = pixelVector_ptr;
  updateLocation();
}

int Centroid::getID()
{
  return m_id;
}

std::vector<Pixel *> Centroid::getOwnedPixels_ptr()
{
  return m_ownedPixels_ptr;
}

Pixel Centroid::getLocation()
{
  return m_location;
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

void Centroid::addPixel(Pixel *pixel_ptr)
{
  m_ownedPixels_ptr.push_back(pixel_ptr);
}

Pixel* Centroid::releasePixel(int pixelID)
{
  Pixel *tempPixel{};
  for(std::size_t i{0}; i < m_ownedPixels_ptr.size(); ++i)
    {
      if(m_ownedPixels_ptr[i]->id == pixelID)
	{
	  tempPixel = m_ownedPixels_ptr.at(i);
	  m_ownedPixels_ptr.erase(m_ownedPixels_ptr.begin() + i);
	  return tempPixel;
	}
    }
  return nullptr;
}

Pixel* Centroid::getPixelByID(int pixelID)
{
  for(auto pixel : m_ownedPixels_ptr)
    {
      if(pixel->id == pixelID)
	{
	  return pixel;
	}
    }
  return nullptr;
}
