#include "videoProcess.h"
#include "pixel.h"
#include "centroid.h"
#include "kmeans.h"
#include "image.h"
#include "color.h"
#include "difference.h"
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
#include <chrono>

// TODO
// Find a good way to calculate the optimal # of frames to sample for
// elbow and accuracy tests (double percent)

void processVideo(std::string filename, int centroids)
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

  std::cout << "Finding best ratio...\n";
  double ratio = findBestRatio(filename, 0.5);

  if(centroids <= 1 || centroids > MAX_CENTROIDS)
    {
      std::cout << "Finding optimal number of centroids...\n";
      centroids = findElbow(filename, ratio);
      // centroids = 3;
      std::cout << "Optimal number of centroids is " << centroids << "\n";
    }

  processVideoLoop(filename, ratio, centroids);

  std::cout << "100%\n";
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
  video.release();
}

void processFrame(cv::Mat* frame, int centroids, int currentFrame)
{
  std::ofstream output{};
  output.open(PROJECT_PATH + "output", std::ios_base::app);
  assert(!output.fail());

  // Needs changing
  (void)currentFrame;

  std::vector<Pixel> pixelVector{};
  scanImage(*frame, pixelVector);
  std::vector<Centroid> centroidVector{};
  createAndProcessCentroids(pixelVector, centroidVector, centroids, 20);

  for(auto &centroid : centroidVector)
    {
      centroid.printLocation(true, output, frame->rows * frame->cols);
    }

  output << "\n";
  output.close();
}

int findElbow(std::string filename, double ratio)
{
  cv::VideoCapture video(filename, cv::CAP_FFMPEG);
  assert(video.isOpened());

  cv::Mat resizedFrame{};

  std::map<int, int> totalElbows{findElbowLoop(filename, 0.5, ratio)};

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
	  double percentComplete{round((static_cast<double>(currentFrame) /
					static_cast<double>(frames)) * 100)};
	  if (percentComplete > last)
	    {
	      std::cout << percentComplete << "% complete\n";

	      last = percent;
	    }

	  cv::resize(frame, resizedFrame, cv::Size(), ratio, ratio, cv::INTER_LANCZOS4);
	  totalElbows[findElbowFrame(&resizedFrame)]++;
	}
      ++currentFrame;
    }
  return totalElbows;
}

int findElbowFrame(cv::Mat* frame)
{
  std::vector<Pixel> pixelVector{};
  scanImage(*frame, pixelVector);
  std::vector<double> distortionVector{};
  std::vector<double> absoluteChange{};

  for(int i{1}; i <= MAX_CENTROIDS; ++i)
    {
      std::vector<Centroid> centroidVector{};
      createAndProcessCentroids(pixelVector, centroidVector, i, 20);
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
  double totalProcessTimeLarge{};
  double totalProcessTimeSmall{};
  double averageProcessTimeSmall{};
  double averageProcessTimeLarge{};
  double largestDifference{};
  std::vector<double> largestDifferenceVector{};
  std::vector<double> averageDifferenceVector{};
  std::vector<double> totalTimeVector{};
  std::vector<double> averageTimeVector{};
  std::vector<int> lostCentroids{};
  const int minRatio{1};
  const int maxRatio{8};

  std::vector<Color> colorVectorLarge{};
  extractColor(filename, percent, maxRatio, totalProcessTimeLarge,
	       averageProcessTimeLarge, colorVectorLarge);
  for (int ratio{minRatio}; ratio < maxRatio; ++ratio)
    {
      std::vector<Color> colorVectorSmall{};
      extractColor(filename, percent, ratio, totalProcessTimeSmall,
		   averageProcessTimeSmall, colorVectorSmall);
      double averageDifference {compareAccuracy(colorVectorLarge, colorVectorSmall, largestDifference)};
      std::cout << "The average color difference between ratios of " << maxRatio << " and " << ratio << " is " << averageDifference << "\n";
      std::cout << "The largest color difference between ratios of " << maxRatio << " and " << ratio << " is " << largestDifference << "\n";
      std::cout << "The average speed difference (per frame) is " << averageProcessTimeLarge - averageProcessTimeSmall << "\n";

      largestDifferenceVector.push_back(largestDifference);
      averageDifferenceVector.push_back(averageDifference);
      totalTimeVector.push_back(totalProcessTimeSmall);
      averageTimeVector.push_back(averageProcessTimeSmall);
      lostCentroids.push_back(colorVectorLarge.size() - colorVectorSmall.size());
    }
  return 0.08;
}

void extractColor(std::string filename, double percent,
		  double ratio, double &totalProcessTime_o,
		  double &averageProcessTime_o, std::vector<Color> &colorVector)
{
  colorVector.clear();
  totalProcessTime_o = 0.0;
  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::duration;
  using std::chrono::milliseconds;
  ratio /= 100.0;

  totalProcessTime_o = 0;
  auto startTime = high_resolution_clock::now();

  int counter {extractColorLoop(filename, percent, ratio, colorVector)};
  auto endTime = high_resolution_clock::now();
  duration<double, std::milli> totalProcessTime = duration_cast<milliseconds>
    (endTime - startTime);

  totalProcessTime_o = totalProcessTime.count() / 1000;
  averageProcessTime_o = totalProcessTime_o / counter;
}

int extractColorLoop(std::string filename, double percent, double ratio,
		     std::vector<Color> &colorVector)
{
  colorVector.clear();
  cv::VideoCapture video{filename, cv::CAP_FFMPEG};

  cv::Mat resizedFrame{};
  int currentFrame{1};
  double frames{video.get(cv::CAP_PROP_FRAME_COUNT)};
  int counter{10};
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
	  double percentComplete{floor(((static_cast<double>(currentFrame) / frames)) * 100)};
	  if(percentComplete > last)
	    {
	      // std::cout << percentComplete << "% complete\n";
	      last = percentComplete;
	    }

	  cv::resize(frame, resizedFrame, cv::Size(), ratio, ratio, cv::INTER_LANCZOS4);

	  for(auto &color : extractColorFrame(&resizedFrame, currentFrame))
	    {
	      colorVector.push_back(color);
	    }
	  ++counter;
	}
      ++currentFrame;
    }
  return counter;
}

std::vector<Color> extractColorFrame(cv::Mat *frame, int currentFrame)
{
  std::vector<Pixel> pixelVector{};
  scanImage(*frame, pixelVector);
  std::vector<Centroid> centroidVector{};
  createAndProcessCentroids(pixelVector, centroidVector, 3, 20);

  std::vector<Color> colorVector{};

  for(std::size_t i{0}; i < centroidVector.size(); ++i)
    {
      if(centroidVector[i].getLocation().r >= 0 || centroidVector[i].getLocation().g >= 0 ||
	 centroidVector[i].getLocation().b >= 0)
	{
	  colorVector.push_back(Color{static_cast<double>(centroidVector[i].getLocation().r),
				      static_cast<double>(centroidVector[i].getLocation().g),
				      static_cast<double>(centroidVector[i].getLocation().b),
				      currentFrame, static_cast<int>(i)});
	}

    }

  return colorVector;

}

double compareAccuracy(std::vector<Color> colorVectorLarge,
		       std::vector<Color> colorVectorSmall,
		       double &largestDifference_o)
{
  double totalDifference{};
  largestDifference_o = 0.0;
  std::size_t totalColors{colorVectorLarge.size()};
  Color largestColorDifference{};
  Color smallestColorDifference{};
  largestDifference_o = 0;

  for(std::size_t i {0}; i < totalColors; ++i)
    {
      if(colorVectorLarge[i].frame() == colorVectorSmall[i].frame() &&
	 colorVectorLarge[i].centroidIndex() == colorVectorSmall[i].centroidIndex())
	{
	  double difference = abs(deltaE00Difference(colorVectorLarge[i], colorVectorSmall[i]));
	  if(difference > largestDifference_o)
	    {
	      largestColorDifference = colorVectorLarge[i];
	      smallestColorDifference = colorVectorSmall[i];
	      largestDifference_o = difference;
	    }
	  totalDifference += difference;
	}
    }
  return abs(totalDifference / static_cast<double>(totalColors));
}
