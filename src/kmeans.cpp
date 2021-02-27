#include "kmeans.h"
#include "pixel.h"
#include "centroid.h"

#include <vector>

std::vector<Centroid> createCentroids(std::vector<Pixel> pixelVector, int k)
{
  std::vector<Centroid> centroidVector{};

  int pixelsPerCentroid{static_cast<int>(pixelVector.size()) / k};
  int leftoverPixels{static_cast<int>(pixelVector.size()) % k};

  for(int i{0}; i < k; ++i)
    {
      Centroid tempCentroid{};
      for(int j{pixelsPerCentroid * i}; j < (pixelsPerCentroid * (i + 1)); ++j)
	{
	  tempCentroid.addPixel(&pixelVector[j]);
	}
      centroidVector.push_back(tempCentroid);
    }

  if(leftoverPixels != 0)
    {
      for(int i{static_cast<int>(pixelVector.size()) - leftoverPixels};
	  i < static_cast<int>(pixelVector.size()); ++i)
	{
	  centroidVector[centroidVector.size() - 1].addPixel(&pixelVector[i]);
	}
    }
  return centroidVector;
}
