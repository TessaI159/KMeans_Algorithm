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
bool updateCentroidOwnership(std::vector<Centroid> & centroidVector,
			     const std::vector<Pixel> &pixelVector);

// Calls updateLocation and updateCentroidOwnership
bool updateCentroids(std::vector<Centroid> &centroidVector,
		     const std::vector<Pixel> &pixelVector);

// Returns the smallest element in an array with length size
int smallestElement(double distances[], std::size_t size);

// 
bool swap(std::vector<Centroid> &centroidVector, Pixel* pixel, int centroidToSwapID);

void createAndProcessCentroids(std::vector<Pixel> &pixelVector,
						std::vector<Centroid> &centroidVector,
						int numCentroids, int maxIter);

#endif
