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
#include <math.h>
#include <array>
#include <map>

void processVideo(std::string filename, int centroids)
{
  if(centroids < 1 || centroids > 10)
    {
      // centroids = 3;
      centroids = findElbow(filename);
    }

  cv::VideoCapture video(filename, cv::CAP_FFMPEG);
  assert(video.isOpened());

  double ratio = findRatio(5000, 2500, 0.08,
			   video.get(cv::CAP_PROP_FRAME_WIDTH),
			   video.get(cv::CAP_PROP_FRAME_HEIGHT));

  int index{0};
  int frames(video.get(cv::CAP_PROP_FRAME_COUNT));
  cv::Mat frame{};
  // cv::namedWindow("Processing", cv::WINDOW_AUTOSIZE);
  cv::Mat resizedFrame{};

  while(true)
    {
      video >> frame;

      if(frame.empty())
	{
	  break;
	}
      cv::resize(frame, resizedFrame, cv::Size(), ratio, ratio, cv::INTER_LANCZOS4);
      assert(resizedFrame.data);
      std::cout << index << "/" << frames << "\n";
      // cv::imshow("Processing", frame);
      // char c{static_cast<char>(cv::waitKey(25))};
      // if(c == 27)
      //        {
      //          break;
      //        }
      processFrame(&resizedFrame, centroids);
      index++;
    }
  video.release();
}

void processFrame(cv::Mat* frame, int centroids)
{
  std::vector<Pixel> pixelVector{scanImage(*frame)};
  std::vector<Centroid> centroidVector{createCentroids(pixelVector, centroids)};
  int maxIter{20};
  int currentIter{1};
  
  while(updateCentroids(centroidVector, pixelVector))
    {
      if(currentIter >= maxIter)
	{
	  break;
	}
      currentIter++;
    }
}

int findElbow(std::string filename)
{
  cv::VideoCapture video(filename, cv::CAP_FFMPEG);
  assert(video.isOpened());

  double ratio = findRatio(5000, 2500, 0.08,
			   video.get(cv::CAP_PROP_FRAME_WIDTH),
			   video.get(cv::CAP_PROP_FRAME_HEIGHT));
  cv::Mat resizedFrame{};

  int currentFrame{1};
  int seconds{5};
  // int frames(video.get(cv::CAP_PROP_FRAME_COUNT));
  double fps{video.get(cv::CAP_PROP_FPS)};
  fps = std::round(fps);
  std::map<int, int> totalElbows{};
  for(int i{1}; i < 11; ++i)
    {
      totalElbows[i] = 0;
    }

  while(true)
    {
      cv::Mat frame;
      video >> frame;

      if(frame.empty())
	{
	  break;
	}

      if(fmod(static_cast<double>(currentFrame), fps * seconds) == 0)
	{
	  cv::resize(frame, resizedFrame, cv::Size(), ratio, ratio, cv::INTER_LANCZOS4);
	  totalElbows[findElbowFrame(&resizedFrame)]++;
	}
      ++currentFrame;
    }
  int largest{totalElbows[1]};
  int largestIndex{1};
  for(std::size_t i{2}; i < totalElbows.size(); ++i)
    {
      if(totalElbows[i] > largest)
	{
	  largestIndex = i;
	}
    }
  video.release();
  return largestIndex;
}

// I'm 99% sure there's a more elegant, less cpu and memory intensive way to do this
int findElbowFrame(cv::Mat* frame)
{
  std::vector<Pixel> pixelVector{scanImage(*frame)};
  std::vector<Centroid> centroidVector{};
  std::vector<double> distortionVector{};
  std::vector<double> absoluteChange{};

  for(int i{1}; i <= 10 + 1; ++i)
    {
      centroidVector = createCentroids(pixelVector, i);
      while(updateCentroids(centroidVector, pixelVector)){}
      double averageDistortion{0.0};
      
      for(auto &centroid : centroidVector)
	{
	  averageDistortion += centroid.distortion();
	}
      averageDistortion /= centroidVector.size();
      distortionVector.push_back(averageDistortion);
    }
  
  for(std::size_t i{0}; i < distortionVector.size() - 1; ++i)
    {
      absoluteChange.push_back(distortionVector[i] - distortionVector[i+1]);
    }

  for(std::size_t i{1}; i < absoluteChange.size() - 1; ++i)
    {
      if(absoluteChange[i] / absoluteChange[0] <= 0.065)
	{
	  return i + 1;
	}
    }
  
  
  return 0;
}

double findRatio(int targetPixels, int minPixels, double ratio, double width, double height)
{
  targetPixels = 5000;
  minPixels = 2500;
  ratio = 0.08;

  if(width * height * ratio * ratio > targetPixels ||
     width * height * ratio * ratio < minPixels)
    {
      ratio = sqrt(static_cast<double>(targetPixels) /
		   (static_cast<double>(height) *
		    static_cast<double>(width)));
    }
  return ratio;
}
