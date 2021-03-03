#include "pixel.h"
#include "image.h"
#include "centroid.h"
#include "kmeans.h"

#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>


int main()
{
  // clock_t totalTimeStart{clock()};

  cv::Mat loadedImage = cv::imread("/home/tess/Code/KMeans/assets/new_test.jpg");
  assert(loadedImage.data);

  cv::Mat lanczos4{};

  cv::resize(loadedImage, lanczos4, cv::Size(), 0.08, 0.08, cv::INTER_LANCZOS4);
  

  clock_t totalTimeStart_25{clock()};

  std::vector<Pixel> pixelVector_25{scanImage(lanczos4)};
  assert(lanczos4.data);
  std::vector<Centroid> centroidVector_25{createCentroids(pixelVector_25, 3)};

  // int updateNumber_25{0};
  // std::cout << "Update number " << ++updateNumber_25 << "\n";
  while(updateCentroids(centroidVector_25, pixelVector_25))
    {
      // std::cout << "Update number " << ++updateNumber_25 << "\n";
    }

  for(auto &centroid : centroidVector_25)
    {
      centroid.printLocation(true);
    }

  clock_t totalTimeEnd_25{clock()};
  std::cout << "Program took a total of " << (totalTimeEnd_25 - totalTimeStart_25) / static_cast<double>(CLOCKS_PER_SEC);
  std::cout << " for a total of " << pixelVector_25.size() << " pixels, totaling " << ((totalTimeEnd_25 - totalTimeStart_25) / static_cast<double>(CLOCKS_PER_SEC)) / pixelVector_25.size();
  std::cout << " on average per pixel\n";

  return 0;
}
