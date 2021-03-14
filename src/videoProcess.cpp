#include "videoProcess.h"
#include "pixel.h"
#include "centroid.h"
#include "kmeans.h"
#include "image.h"
#include "color.h"
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

// TODO
// Find a good way to calculate the optimal # of frames to sample for
// elbow and accuracy tests

void processVideo(std::string filename, int centroids, int targetPixels, int minPixels)
{
  std::string output = "output";
  std::string outputFilename{PROJECT_PATH + "output", std::ios_base::app};
  std::ifstream outputCheck{};
  outputCheck.open(outputFilename);
  if(!outputCheck.fail())
    {
      outputCheck.close();
      std::remove(outputFilename.c_str());
    }

  if(centroids < 1 || centroids > MAX_CENTROIDS)
    {
      std::cout << "Finding optimal number of centroids...\n";
      centroids = findElbow(filename, targetPixels, minPixels);
      // centroids = 3;
      std::cout << "Optimal number of centroids is " << centroids << "\n";
    }

  cv::VideoCapture video(filename, cv::CAP_FFMPEG);
  assert(video.isOpened());

  double ratio = findRatio(targetPixels, minPixels, 0.08,
			   video.get(cv::CAP_PROP_FRAME_WIDTH),
			   video.get(cv::CAP_PROP_FRAME_HEIGHT));


  processVideoLoop(filename, ratio, centroids);

  std::cout << "100%\n";
  video.release();
}

void processVideoLoop(std::string filename, double ratio, int centroids)
{
  cv::VideoCapture video{filename, cv::CAP_FFMPEG};
  assert(video.isOpened());

  int index{0};
  int frames(video.get(cv::CAP_PROP_FRAME_COUNT));
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
}

void processFrame(cv::Mat* frame, int centroids, int currentFrame)
{
  std::ofstream output{};
  output.open(PROJECT_PATH + "output");
  assert(!output.fail());

  std::vector<Pixel> pixelVector{scanImage(*frame)};
  std::vector<Centroid> centroidVector{createAndProcessCentroids(pixelVector, centroids, 20)};

  for(auto &centroid : centroidVector)
    {
      centroid.printLocation(true, output, frame->rows * frame->cols);
    }
  output << "\n";
  output.close();
}

int findElbow(std::string filename, int targetPixels, int minPixels)
{
  cv::VideoCapture video(filename, cv::CAP_FFMPEG);
  assert(video.isOpened());

  cv::Mat resizedFrame{};

  double ratio {findRatio(targetPixels, minPixels, 0.08,
			  video.get(cv::CAP_PROP_FRAME_WIDTH),
			  video.get(cv::CAP_PROP_FRAME_HEIGHT))};

  std::map<int, int> totalElbows{findElbowLoop(filename, 0.15, ratio)};

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

std::map<int, int> findElbowLoop(std::string filename, double percent, double ratio)
{

  cv::VideoCapture video(filename, cv::CAP_FFMPEG);
  assert(video.isOpened());

  percent *= 100;
  int currentFrame{1};
  int frames(video.get(cv::CAP_PROP_FRAME_COUNT));
  std::map<int, int> totalElbows{};
  cv::Mat resizedFrame{};
  double last{0};

  for(int i{1}; i <= MAX_CENTROIDS; ++i)
    {
      totalElbows[i] = 0;
    }


  while(true)
    {
      cv::Mat frame{};
      video >> frame;

      if(frame.empty())
	{
	  break;
	}

      if(fmod(static_cast<double>(currentFrame), round(100.0 / percent)) == 0)
	{
	  double percent {round((static_cast<double>(currentFrame) /
				 static_cast<double>(frames)) * 100)};
	  if (percent > last)
	    {
	      std::cout << percent << "% complete\n";

	      last = percent;
	    }

	  cv::resize(frame, resizedFrame, cv::Size(), ratio, ratio, cv::INTER_LANCZOS4);
	  totalElbows[findElbowFrame(&resizedFrame)]++;
	}
      ++currentFrame;
    }
  return totalElbows;
}

// I'm 99% sure there's a more elegant, less cpu and memory intensive way to do this
int findElbowFrame(cv::Mat* frame)
{
  std::vector<Pixel> pixelVector{scanImage(*frame)};
  std::vector<double> distortionVector{};
  std::vector<double> absoluteChange{};

  for(int i{1}; i <= MAX_CENTROIDS; ++i)
    {
      std::vector<Centroid> centroidVector{createAndProcessCentroids(pixelVector, i, 20)};
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

double findBestRatio(std::string filename, double percent)
{
  std::vector<Color> colorVector15{processRatio(filename, percent, 15)};
  for (int ratio{14}; ratio >= 1; --ratio)
    {
      std::vector<Color> smallerColorVector{processRatio(filename, percent, ratio)};
    }

  return 0.0;
}

std::vector<Color> processRatio(std::string filename, double percent, double ratio)
{
  ratio /= 100.0;
  cv::VideoCapture video{filename, cv::CAP_FFMPEG};
  assert(video.isOpened());
  cv::Mat resizedFrame{};
  int currentFrame{1};
  double frames{video.get(cv::CAP_PROP_FRAME_COUNT)};
  std::vector<Color> colorVector{};

  while(true)
    {
      cv::Mat frame{};

      video >> frame;

      if(frame.empty())
	{
	  break;
	}

      double last {0};
      if(fmod(static_cast<double>(currentFrame), round(100.0 / percent)) == 0)
	{
	  double percent{round((static_cast<double>(currentFrame) /
				frames)) * 100};
	  if(percent > last)
	    {
	      std::cout << percent << "% complete\n";
	      last = percent;
	    }

	  cv::resize(frame, resizedFrame, cv::Size(), ratio, ratio, cv::INTER_LANCZOS4);
	  colorVector.push_back(processRatioFrame(&resizedFrame));
	}
    }
  return colorVector;
}

Color processRatioFrame(cv::Mat *frame)
{
  std::vector<Pixel> pixelVector{scanImage(*frame)};
  std::vector<Centroid> centroidVector{createAndProcessCentroids(pixelVector, 3, 20)};
  
}

double findRatio(int targetPixels, int minPixels, double ratio, double width, double height)
{
  if(width * height * ratio * ratio > targetPixels ||
     width * height * ratio * ratio < minPixels)
    {
      ratio = sqrt(static_cast<double>(targetPixels) /
		   (static_cast<double>(height) *
		    static_cast<double>(width)));
    }
  return ratio;
}
