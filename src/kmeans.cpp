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

void updateCentroidLocations(std::vector<Centroid> &centroidVector)
{
  for(auto &centroid : centroidVector) {
    centroid.updateLocation();
  }
}

bool updateCentroidOwnership(std::vector<Centroid> & centroidVector,
			     const std::vector<Pixel> &pixelVector_c)
{
  for (auto pixel : pixelVector_c)
    {
      double distances[centroidVector.size()];
      for(std::size_t i{0}; i < centroidVector.size(); ++i)
	{
	  distances[i] = centroidVector[i].distanceFromPixel(&pixel);
	}
      if(pixel.ownedBy != smallestElement(distances, centroidVector.size()))
	{
	  centroidVector[pixel.ownedBy].getPixelByID(pixel.id);
	}
    }
}

bool updateCentroids(std::vector<Centroid> &centroidVector,
		     const std::vector<Pixel> &pixelVector_c)
{
  updateCentroidLocations(centroidVector);
  return updateCentroidOwnership(centroidVector, pixelVector_c);
}

int smallestElement(double distances[], std::size_t size)
{
  int smallestElementIndex = 0;
  double smallestElementSize = distances[0];
  for(std::size_t i{1}; i < size; ++i)
    {
      if(distances[i] < smallestElementSize)
	{
	  smallestElementIndex = i;
	}
    }
  return smallestElementIndex;
}
