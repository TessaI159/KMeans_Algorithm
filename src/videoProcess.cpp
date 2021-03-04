#include "videoProcess.h"
#include "pixel.h"
#include "centroid.h"
#include "kmeans.h"
#include "image.h"

#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>
#include <vector>

void processVideo(std::string filename, int centroids, double ratio)
{
  cv::VideoCapture video(filename, cv::CAP_FFMPEG);
  assert(video.isOpened());

  int index{0};
  int frames(video.get(cv::CAP_PROP_FRAME_COUNT));

  while(true)
    {
      // clock_t frameCreationStart{clock()};
      cv::Mat frame;
      video >> frame;
      // clock_t frameCreationEnd{clock()};
      // std::cout << "Frame creation: " <<  (static_cast<double>(frameCreationEnd
      // 							   - frameCreationStart) /
      // 				       static_cast<double>(CLOCKS_PER_SEC)) << "\n";
      if(frame.empty())
	{
	  break;
	}
      std::cout << ++index << "/" << frames << "\n";
      clock_t start{clock()};
      processFrame(frame, centroids, ratio);
      clock_t end{clock()};

      std::cout << "Frame time: " <<  (static_cast<double>(end - start) /
				       static_cast<double>(CLOCKS_PER_SEC)) << "\n";
    }
  video.release();
}

void processVideo(std::string filename)
{
  processVideo(filename, 3, 0.08);
}

void processFrame(cv::Mat frame, int centroids, double ratio)
{
  cv::Mat resizedFrame{};

  cv::resize(frame, resizedFrame, cv::Size(), ratio, ratio, cv::INTER_LANCZOS4);
  assert(resizedFrame.data);

  std::vector<Pixel> pixelVector{scanImage(resizedFrame)};
  std::vector<Centroid> centroidVector{createCentroids(pixelVector, centroids)};

  clock_t updateStart{clock()};
  while(updateCentroids(centroidVector, pixelVector)){}
  clock_t updateEnd{clock()};

  // std::cout << "Centroid update: " <<  (static_cast<double>(updateEnd - updateStart) /
  //            static_cast<double>(CLOCKS_PER_SEC)) << "\n";


}
