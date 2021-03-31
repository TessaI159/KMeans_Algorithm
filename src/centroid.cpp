#include "centroid.h"
#include "pixel.h"

#include <math.h>
#include <iostream>
#include <cassert>
#include <fstream>

Centroid::Centroid(int id) { m_id = id; };

Centroid::Centroid(std::vector<Pixel *> pixelVector_ptr)
{
  m_ownedPixels_ptr = pixelVector_ptr;
  updateLocation();
}

int Centroid::getID()
{
  return m_id;
}

Pixel Centroid::getLocation()
{
  return m_location;
}

int Centroid::getOwnedPixels_ptrSize()
{
  return m_ownedPixels_ptr.size();
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
}

double Centroid::distanceFromPixel(Pixel *pixel_ptr)
{
  if(pixel_ptr == nullptr)
    {
      std::cerr << "Null pixel encountered during distance computing\n";
      exit(EXIT_FAILURE);
    }

  return (pow(static_cast<double>(m_location.r) - static_cast<double>(pixel_ptr->r), 2) +
	  pow(static_cast<double>(m_location.g) - static_cast<double>(pixel_ptr->g), 2) +
	  pow(static_cast<double>(m_location.b) - static_cast<double>(pixel_ptr->b), 2));
}

void Centroid::addPixel(Pixel *pixel_ptr)
{
  assert(pixel_ptr != nullptr);
  m_ownedPixels_ptr.push_back(pixel_ptr);
  (*pixel_ptr).ownedBy = m_id;
}

Pixel* Centroid::releasePixel(int pixelID)
{
  Pixel *tempPixel{};
  for(std::size_t i{0}; i < m_ownedPixels_ptr.size(); ++i)
    {
      if(m_ownedPixels_ptr[i]->id == pixelID)
	{
	  tempPixel = m_ownedPixels_ptr[i];
	  m_ownedPixels_ptr.erase(m_ownedPixels_ptr.begin() + i);
	  (*tempPixel).ownedBy = -1;
	  return tempPixel;
	}
    }
  std::cerr << "Pixel with id " << pixelID << " not found.\n";
  exit(EXIT_FAILURE);
}

void Centroid::printLocation(bool newLine, std::ofstream& output, double pixels)
{
  output << m_id << ": ";
  output << m_location.r << ", ";
  output << m_location.g << ", ";
  output << m_location.b << ", ";
  output << "with " << static_cast<double>(m_ownedPixels_ptr.size()) / pixels;
  if(newLine)
    {
      output << "\n";
    }
}

void Centroid::printLocation(bool newLine)
{
  std::cout << "Centroid " <<  m_id << " at location: (";
  std::cout << m_location.r << ", ";
  std::cout << m_location.g << ", ";
  std::cout << m_location.b << ")";
  std::cout << " with " << m_ownedPixels_ptr.size() << " pixels.";
  if(newLine)
    {
      std::cout << "\n";
    }
}

double Centroid::distortion()
{
  double totalDistance{0};
  for(auto &pixel : m_ownedPixels_ptr)
    {
      double distance{distanceFromPixel(pixel)};
      if(distance == -1)
	{
	  std::cerr << "Distortion method received a nullptr\n";
	  exit(EXIT_FAILURE);
	}
      else
	{
	  totalDistance += distance;
	}
    }

  return totalDistance;
}

