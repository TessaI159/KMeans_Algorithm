#ifndef KMEANS_H
#define KMEANS_H

#include "pixel.h"
#include "centroid.h"

#include <vector>

std::vector<Centroid> createCentroids(std::vector<Pixel> pixelVector, int k);

#endif
