#include "pixel.h"
#include "image.h"
#include "centroid.h"
#include "kmeans.h"

#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>


int main()
{
  clock_t totalTimeStart{clock()};
  cv::Mat loadedImage = cv::imread("/home/tess/Code/KMeans/assets/flower.jpg");
  std::vector<Pixel> pixelVector{scanImage(loadedImage)};
  assert(loadedImage.data);

  
  std::vector<Centroid> centroidVector{createCentroids(pixelVector, 3)};

  int updateNumber{0};
  std::cout << "Update number " << ++updateNumber << "\n";
  while(updateCentroids(centroidVector, pixelVector))
    {
      std::cout << "Update number " << ++updateNumber << "\n";
    }
  
  for(auto &centroid : centroidVector)
    {
      centroid.printLocation(true);
    }
  clock_t totalTimeEnd{clock()};

  std::cout << "Program took a total of " << (totalTimeEnd - totalTimeStart) / static_cast<double>(CLOCKS_PER_SEC);
  std::cout << " for a total of " << pixelVector.size() << " pixels, totaling " << ((totalTimeEnd - totalTimeStart) / static_cast<double>(CLOCKS_PER_SEC)) / pixelVector.size();
  std::cout << " on average per pixel\n";
  return 0;
}
