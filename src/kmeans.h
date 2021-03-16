#ifndef KMEANS_H
#define KMEANS_H

#include "pixel.h"
#include "centroid.h"

#include <vector>

// Takes a number of centroids to create, k, a vector of Pixels,
// returned by scanImage (image.h), and an empty vector
// of Centroids. If the centroid vector is not empty,
// it will be cleared and refilled during this function.
// Creates a vector of k centroids, each with pixelVector.size() / k
// pixels in their m_ownedPixels_ptr vector (centroid.h)
// Any leftover pixels are added to the end of the last centroid in the vector
void createCentroids(std::vector<Pixel> &pixelVector,
				      std::vector<Centroid> &centroidVector, int k);

// Calls updateLocation (centroid.h) on each centroid in the vector
void updateCentroidLocations(std::vector<Centroid> &centroidVector);

// Checks every pixel's distance from each vector. If the closest
// vector does not own the pixels, swap the pixels ownership to it
// Retuns true if changes were made, false otherwise
bool updateCentroidOwnership(std::vector<Centroid> & centroidVector,
			     const std::vector<Pixel> &pixelVector);

// Calls updateLocation and updateCentroidOwnership
bool updateCentroids(std::vector<Centroid> &centroidVector,
		     const std::vector<Pixel> &pixelVector);

// Returns the smallest element in an array with length size
int smallestElement(double distances[], std::size_t size);

// Checks if the pixel pointer passed in is null first, and returns false if it is
// Removes a pixel from &centroidVector and adds it to centroid with centroidToSwapID
// If the pixel pointer is not found in centroidVector, returns false
bool swap(std::vector<Centroid> &centroidVector, Pixel* pixel, int centroidToSwapID);


// Runs createCentroids and then loops through updateCentroids as long as
// it returns true. If it still returns true after maxIter iterations
// break from the loop and consider it completed.
// Standard value for maxIter is 20
void createAndProcessCentroids(std::vector<Pixel> &pixelVector,
						std::vector<Centroid> &centroidVector,
						int numCentroids, int maxIter);

#endif
