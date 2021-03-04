#include "pixel.h"
#include "image.h"
#include "centroid.h"
#include "kmeans.h"

#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>

// TODO
// Code that automatically chooses the optimal number of centroids
// Code that automatically chooses the optimal resize

int main()
{
  clock_t totalTimeStart{clock()};

  cv::Mat loadedImage = cv::imread("/home/tess/Code/KMeans/assets/test_image_big.jpg");
  assert(loadedImage.data);
  cv::VideoCapture vid("/home/tess/Code/KMeans/assets/color.avi", cv::CAP_FFMPEG);
  assert (vid.isOpened());
  
  cv::Mat resizedImage{};
  cv::resize(loadedImage, resizedImage, cv::Size(), 0.08, 0.08, cv::INTER_LANCZOS4);
  assert(resizedImage.data);
  
  std::vector<Pixel> pixelVector{scanImage(resizedImage)};
  // std::vector<Pixel> pixelVectorOG{scanImage(loadedImage)};
  std::vector<Centroid> centroidVector{createCentroids(pixelVector, 5)};

  // int updateNumber{0};
  // std::cout << "Update number " << ++updateNumber << "\n";
  while(updateCentroids(centroidVector, pixelVector))
    {
      // std::cout << "Update number " << ++updateNumber << "\n";
    }

  for(auto &centroid : centroidVector)
    {
      centroid.printLocation(true);
    }
  clock_t totalTimeEnd{clock()};

  std::cout << "Program took a total of " << (totalTimeEnd - totalTimeStart) / static_cast<double>(CLOCKS_PER_SEC);
  std::cout << " for a total of " << pixelVector.size() << " pixels, totaling " << ((totalTimeEnd - totalTimeStart) / static_cast<double>(CLOCKS_PER_SEC)) / pixelVector.size();
  std::cout << " on average per pixel\n";

  while(true)
    {
      cv::Mat frame;
      vid >> frame;
      
      if(frame.empty())
	{
	  break;
	}
    }

  vid.release();
  
  return 0;
}
