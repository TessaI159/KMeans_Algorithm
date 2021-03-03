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
  // int index{0};
  std::size_t pixelVectorSize{pixelVector.size()};
  double distancesTotal{0};
  double swapsTotal{0};
  int numSwaps{0};
  for (auto pixel : pixelVector)
    {
      // ++index;
      // std::cout << index << "/" << pixelVectorSize << "\n";
      double distances[centroidVector.size()];
      clock_t distancesStart{clock()};
      for(std::size_t i{0}; i < centroidVector.size(); ++i)
	{
	  distances[i] = centroidVector[i].distanceFromPixel(&pixel);
	}
      clock_t distancesEnd{clock()};
      distancesTotal += static_cast<double>(distancesEnd - distancesStart);
      if(pixel.ownedBy != smallestElement(distances, centroidVector.size()))
	{
	  ++numSwaps;
	  clock_t swapsStart{clock()};
	  updated = true;
	  swap(centroidVector, &pixel, smallestElement(distances, centroidVector.size()));
	  clock_t swapsEnd{clock()};
	  swapsTotal += static_cast<double>(swapsEnd - swapsStart);
	}
    }
  
  double distancesAverage = (distancesTotal / pixelVectorSize) / static_cast<double>(CLOCKS_PER_SEC);
  double swapsAverage = (swapsTotal / numSwaps) / static_cast<double>(CLOCKS_PER_SEC);
  std::cout << "Distance check took an average of: " << distancesAverage << " per pixel.\n";
  std::cout << "Swaps took an average of: " << swapsAverage << " per pixel.\n";
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
  (*pixel_ptr).needsSwapped = false;
  Pixel *tempPixel_ptr {nullptr};
  tempPixel_ptr = centroidVector[pixel_ptr->ownedBy].releasePixel(pixel_ptr->id);
  if(tempPixel_ptr == nullptr)
    {
      return false;
    }
  centroidVector[centroidToSwapID].addPixel(tempPixel_ptr);
  (*pixel_ptr).swapTo = -1;
  return true;
}
