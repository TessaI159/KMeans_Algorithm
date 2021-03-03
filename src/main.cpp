#include "pixel.h"
#include "image.h"
#include "centroid.h"
#include "kmeans.h"

#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>


int main()
{
  cv::Mat loadedImage = cv::imread("/home/tess/Code/KMeans/assets/flower.jpg");
  std::vector<Pixel> pixelVector{scanImage(loadedImage)};

  assert(loadedImage.data);

  std::vector<Centroid> centroidVector{createCentroids(pixelVector, 3)};

  int updateNumber{0};
  while(updateCentroids(centroidVector, pixelVector))
    {
      std::cout << "Update number " << ++updateNumber << "\n";
    }
  
  for(auto &centroid : centroidVector)
    {
      centroid.printLocation(true);
    }
  return 0;
}
