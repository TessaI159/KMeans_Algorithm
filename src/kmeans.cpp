#include "kmeans.h"
#include "pixel.h"
#include "centroid.h"

#include <vector>
#include <iostream>

std::vector<Centroid> createCentroids(std::vector<Pixel> &pixelVector, int k)
{
  std::vector<Centroid> centroidVector{};
  int pixelsPerCentroid{static_cast<int>(pixelVector.size()) / k};
  int leftoverPixels{static_cast<int>(pixelVector.size()) % k};
  int id{-1};

  for(int i{0}; i < k; ++i)
    {
      Centroid tempCentroid{};
      tempCentroid.setID(++id);
      for(int j{pixelsPerCentroid * i}; j < (pixelsPerCentroid * (i + 1)); ++j)
	{
	  tempCentroid.addPixel(&pixelVector[j]);
	}
      tempCentroid.updateLocation();
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
			     const std::vector<Pixel> &pixelVector)
{
  bool updated = false;
  int index{0};
  for (auto pixel : pixelVector)
    {
      ++index;
      double distances[centroidVector.size()];
      for(std::size_t i{0}; i < centroidVector.size(); ++i)
	{
	  distances[i] = centroidVector[i].distanceFromPixel(&pixel);
	}
      if(pixel.ownedBy != smallestElement(distances, centroidVector.size()))
	{
	  updated = true;
	  swap(centroidVector, &pixel, smallestElement(distances, centroidVector.size()));
	}
    }

  return updated;
}

bool updateCentroids(std::vector<Centroid> &centroidVector,
		     const std::vector<Pixel> &pixelVector)
{
  updateCentroidLocations(centroidVector);
  return updateCentroidOwnership(centroidVector, pixelVector);
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
	  smallestElementSize = distances[i];
	}
    }
  return smallestElementIndex;
}


bool swap(std::vector<Centroid> &centroidVector, Pixel* pixel_ptr, int centroidToSwapID)
{
  Pixel *tempPixel_ptr {nullptr};
  tempPixel_ptr = centroidVector[pixel_ptr->ownedBy].releasePixel(pixel_ptr->id);

  if(tempPixel_ptr == nullptr)
    {
      return false;
    }
  centroidVector[centroidToSwapID].addPixel(tempPixel_ptr);
  return true;
}
