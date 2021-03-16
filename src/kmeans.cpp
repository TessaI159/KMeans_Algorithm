#include "kmeans.h"
#include "pixel.h"
#include "centroid.h"
#include "constants.h"

#include <vector>
#include <iostream>
#include <cassert>

void createCentroids(std::vector<Pixel> &pixelVector,
		     std::vector<Centroid> &centroidVector, int k)
{
  centroidVector.clear();
  if(k <= 0 || k > MAX_CENTROIDS)
    {
      std::cout << k;
      std::cerr << " Invalid number of centroids. Defaulting to 3\n";
      k = 3;
    }
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
	  double tempDistance{centroidVector[i].distanceFromPixel(&pixel)};
	  assert(tempDistance >= 0);
	  distances[i] = tempDistance;
	}
      if(pixel.ownedBy != smallestElement(distances, centroidVector.size()))
	{
	  updated = swap(centroidVector, &pixel, smallestElement(distances, centroidVector.size()));
	  if(!updated)
	    {
	      std::cerr << "Error while attempting to swap pixel ownership.";
	      std::cerr << " Immediately moving to next centroid comparison without attempting further changes\n";
	      continue;
	    }
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
  if(pixel_ptr == nullptr)
    {
      std::cerr << "Pixel pointer passed to swap was nullptr\n";
      return false;
    }
  Pixel *tempPixel_ptr {nullptr};
  tempPixel_ptr = centroidVector[pixel_ptr->ownedBy].releasePixel(pixel_ptr->id);

  if(tempPixel_ptr == nullptr)
    {
      std::cerr << "Unable to release pixel " << pixel_ptr->id << " from centroid " << pixel_ptr->ownedBy << "\n";
      return false;
    }
  centroidVector[centroidToSwapID].addPixel(tempPixel_ptr);
  return true;
}


std::vector<Centroid> createAndProcessCentroids(std::vector<Pixel> &pixelVector, int numCentroids, int maxIter)
{
  std::vector<Centroid> centroidVector{};
  createCentroids(pixelVector, centroidVector, numCentroids);
  
  int currentIter{1};

  while(updateCentroids(centroidVector, pixelVector))
    {
      if(currentIter > maxIter)
	{
	  break;
	}

      ++currentIter;
    }
  return centroidVector;
}
