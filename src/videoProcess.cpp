#include "videoProcess.h"
#include "pixel.h"
#include "centroid.h"
#include "kmeans.h"
#include "image.h"

#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

void processVideo(std::string filename, int centroids)
{
  cv::VideoCapture video(filename, cv::CAP_FFMPEG);
  assert(video.isOpened());

  int index{0};
  int frames(video.get(cv::CAP_PROP_FRAME_COUNT));

  while(true)
    {
      cv::Mat frame;
      video >> frame;
      int pixels {frame.rows * frame.cols};
      
      if(frame.empty())
	{
	  break;
	}
      
      std::cout << ++index << "/" << frames << "\n";
      clock_t start{clock()};
      processFrame(&frame, centroids, pixels);
      clock_t end{clock()};

      std::cout << "Frame time: " <<  (static_cast<double>(end - start) /
				       static_cast<double>(CLOCKS_PER_SEC)) << "\n";
    }
  video.release();
}

void processVideo(std::string filename)
{
  processVideo(filename, 3);
}

void processFrame(cv::Mat* frame, int centroids)
{
  int targetPixels{5000};
  int minPixels{2500};
  
  cv::Mat resizedFrame{};
  double ratio{0.08};
  
  if(frame->cols * frame->rows * ratio * ratio > targetPixels ||
     frame->cols * frame->rows * ratio * ratio < minPixels)
    {
      ratio = sqrt(static_cast<double>(targetPixels)/(static_cast<double>(frame->rows) * static_cast<double>(frame->cols)));
    }
  cv::resize(*frame, resizedFrame, cv::Size(), ratio, ratio, cv::INTER_LANCZOS4);
  assert(resizedFrame.data);

  std::vector<Pixel> pixelVector{scanImage(resizedFrame)};
  std::vector<Centroid> centroidVector{createCentroids(pixelVector, centroids)};

  while(updateCentroids(centroidVector, pixelVector)){}
}
