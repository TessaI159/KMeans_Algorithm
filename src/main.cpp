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
  // cv::Mat nearest{};
  // cv::Mat linear{};
  // cv::Mat area{};
  // cv::Mat cubic{};
  // cv::Mat lanczos4{};
  assert(loadedImage.data);

  // cv::resize(loadedImage, nearest, cv::Size(), 0.75, 0.75, cv::INTER_NEAREST);
  
  // cv::resize(loadedImage, linear, cv::Size(), 0.75, 0.75, cv::INTER_LINEAR);
  // cv::resize(loadedImage, area, cv::Size(), 0.75, 0.75, cv::INTER_AREA);
  // cv::resize(loadedImage, cubic, cv::Size(), 0.75, 0.75, cv::INTER_CUBIC);
  // cv::resize(loadedImage, lanczos4, cv::Size(), 0.75, 0.75, cv::INTER_LANCZOS4);
  
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
