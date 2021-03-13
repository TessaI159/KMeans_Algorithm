#include "videoProcess.h"
#include "pixel.h"
#include "centroid.h"
#include "kmeans.h"
#include "image.h"
#include "constants.h"

#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <math.h>
#include <array>
#include <map>
#include <fstream>

// Refactor
void processVideo(std::string filename, int centroids, int targetPixels, int minPixels)
{
  std::string output = "output";
  std::string outputFilename{PROJECT_PATH + "output"};
  std::ifstream outputCheck{};
  outputCheck.open(outputFilename);
  if(!outputCheck.fail())
    {
      outputCheck.close();
      std::remove(outputFilename.c_str());
    }
  
  if(centroids < 1 || centroids > MAX_CENTROIDS)
    {
      centroids = findElbow(filename, targetPixels, minPixels);
      std::cout << "Optimal number of centroids is: " << centroids << "\n";
    }

  cv::VideoCapture video(filename, cv::CAP_FFMPEG);
  assert(video.isOpened());

  double ratio = findRatio(targetPixels, minPixels, 0.08,
			   video.get(cv::CAP_PROP_FRAME_WIDTH),
			   video.get(cv::CAP_PROP_FRAME_HEIGHT));

  int index{0};
  int frames(video.get(cv::CAP_PROP_FRAME_COUNT));
  std::cout << frames << "\n";
  cv::Mat frame{};
  cv::Mat resizedFrame{};
  int last{-1};

  while(true)
    {
      video >> frame;

      if(frame.empty())
	{
	  break;
	}

      cv::resize(frame, resizedFrame, cv::Size(), ratio, ratio, cv::INTER_LANCZOS4);
      assert(resizedFrame.data);
      
      index++;
      processFrame(&resizedFrame, centroids, index);
      
      double percentage = floor(static_cast<double>(static_cast<double>(index) /
						    static_cast<double>(frames)) * 100);
      if(percentage > last)
	{
	  last = static_cast<int>(percentage);
	  std::cout << percentage << "%\n";
	}
    }
  std::cout << "100%\n";
  video.release();
}

void processFrame(cv::Mat* frame, int centroids, int currentFrame)
{
  std::ofstream output{};
  output.open(PROJECT_PATH + "output");
  assert(!output.fail());
  
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
  output << "Frame: " << currentFrame << "\n";
  for(auto &centroid : centroidVector)
    {
      centroid.printLocation(true, output, frame->rows * frame->cols);
    }
  output << "\n";
  output.close();
}

// Refactor
int findElbow(std::string filename, int targetPixels, int minPixels)
{
  cv::VideoCapture video(filename, cv::CAP_FFMPEG);
  assert(video.isOpened());

  double ratio = findRatio(targetPixels, minPixels, 0.08,
			   video.get(cv::CAP_PROP_FRAME_WIDTH),
			   video.get(cv::CAP_PROP_FRAME_HEIGHT));
  cv::Mat resizedFrame{};

  int currentFrame{1};
  double percent{0.5};
  int frames(video.get(cv::CAP_PROP_FRAME_COUNT));
  std::map<int, int> totalElbows{};

  for(int i{1}; i <= MAX_CENTROIDS; ++i)
    {
      totalElbows[i] = 0;
    }

  std::cout << "Testing to find the optimal number of centroids.\n";

  while(true)
    {
      cv::Mat frame;
      video >> frame;

      if(frame.empty())
	{
	  break;
	}

      if(fmod(static_cast<double>(currentFrame), round(100.0 / percent)) == 0)
	{
	  std::cout << round((static_cast<double>(currentFrame) /
			      static_cast<double>(frames)) * 100) << "% complete\n";
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
	  largest = totalElbows[i];
	  largestIndex = i;
	}
    }
  video.release();
  std::cout << "100% complete.\n";
  return largestIndex;
}

// I'm 99% sure there's a more elegant, less cpu and memory intensive way to do this
int findElbowFrame(cv::Mat* frame)
{
  std::vector<Pixel> pixelVector{scanImage(*frame)};
  std::vector<Centroid> centroidVector{};
  std::vector<double> distortionVector{};
  std::vector<double> absoluteChange{};

  for(int i{1}; i <= MAX_CENTROIDS; ++i)
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
