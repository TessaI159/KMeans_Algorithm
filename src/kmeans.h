#ifndef KMEANS_H
#define KMEANS_H

#include "pixel.h"
#include "centroid.h"

#include <vector>

std::vector<Centroid> createCentroids(std::vector<Pixel> pixelVector, int k);

void updateCentroidLocations(std::vector<Centroid> &centroidVector);

bool updateCentroidOwnership(std::vector<Centroid> & centroidVector,
			     const std::vector<Pixel> &pixelVector_c);

bool updateCentroids(std::vector<Centroid> &centroidVector,
		     const std::vector<Pixel> &pixelVector_c);

int smallestElement(double distances[], std::size_t size);

#endif
