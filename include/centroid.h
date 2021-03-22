#ifndef CENTROID_H
#define CENTROID_H

#include "pixel.h"

#include <vector>
#include <fstream>

class Centroid
{
private:
  // Each centroids contains a unique ID
  int m_id{-1};
  
  // A vector of pointers to Pixel objects (pixel.h)
  // The pointers in this vector point to the main vector of pixels
  // populated in image.cpp
  // Each pixel should only have one pointer to it
  // Possibly implementing unique_ptrs for this
  std::vector<Pixel *> m_ownedPixels_ptr{};
  Pixel m_location{};
  
public:
  Centroid();
  
  // Sets m_ownedPixels_ptr equal to pixelVector_ptr
  // and calls updateLocation()
  Centroid(std::vector<Pixel *> pixelVector_ptr);
  
  // Returns the centroids unique m_id
  int getID();
  
  // Changes the centroid's unique m_id
  // Thinking about it, this seems dangerous
  // Any function could technically change a centroid's ID
  void setID(int id);
  
  // Returns m_location
  Pixel getLocation();

  // Returns the number of owned pixels
  int getOwnedPixels_ptrSize();
  
  // Updates the location by finding the average r, g, and b
  // of all pixels pointed to by m_ownedPixels_ptr
  // and sets the r, g, and b of m_location to those averages
  void updateLocation();
  
  // Returns the euclidean distance between m_location and pixel_ptr
  double distanceFromPixel(Pixel *pixel_ptr);
  
  // Adds a pixel pointer to m_ownedPixels_ptr
  // and changes the pixels ownedBy trait to m_id
  void addPixel(Pixel *pixel_ptr);
  
  // Deletes the pixel in m_ownedPixels_ptr with id matching pixelID
  // Returns a pointer to the pixel with pixelID
  // If no pixel with pixelID is found, returns nullptr
  Pixel* releasePixel(int pixelID);
  
  // Prints m_location out to output including the ratio of pixels it owns
  // newLine decides whether to print a newLine after outputting location
  // Output is the output stream
  // Pixels is the total number of pixels in the main pixelVector
  void printLocation(bool newLine, std::ofstream& output, double pixels);
  
  // Same as above without the optin to print ratio
  // Will always print to stdout
  void printLocation(bool newLine);

  // Returns the centroids distortion
  // Which is equal to the sum of squared errors
  // of each of its pixels
  double distortion();
};

#endif
