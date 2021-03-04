#include "videoProcess.h"
#include "pixel.h"
#include "centroid.h"
#include "kmeans.h"
#include "image.h"

#include <opencv2/opencv.hpp>

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
      cv::Mat frame;
      video >> frame;

      if(frame.empty())
	{
	  break;
	}
      std::cout << ++index << "/" << frames << "\n";
      processFrame(frame, centroids, ratio);
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
  
  while(updateCentroids(centroidVector, pixelVector)){}
  
  
}
