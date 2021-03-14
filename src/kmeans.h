#ifndef KMEANS_H
#define KMEANS_H

#include "pixel.h"
#include "centroid.h"

#include <vector>

std::vector<Centroid> createCentroids(std::vector<Pixel> &pixelVector, int k);

void updateCentroidLocations(std::vector<Centroid> &centroidVector);

bool updateCentroidOwnership(std::vector<Centroid> & centroidVector,
			     const std::vector<Pixel> &pixelVector);

bool updateCentroids(std::vector<Centroid> &centroidVector,
		     const std::vector<Pixel> &pixelVector);

int smallestElement(double distances[], std::size_t size);

bool swap(std::vector<Centroid> &centroidVector, Pixel* pixel, int centroidToSwapID);

std::vector<Centroid> createAndProcessCentroids(std::vector<Pixel> &pixelVector, int numCentroids, int maxIter);

#endif
