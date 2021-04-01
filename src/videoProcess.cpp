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
#include <math.h>
#include <map>
#include <fstream>
#include <chrono>

void processVideo(std::string filename, int centroids)
{
  std::string outputFilename{filename + "_cache"};
  std::ifstream outputCheck{};
  outputCheck.open(outputFilename);
  if(!outputCheck.fail())
    {
      outputCheck.close();
      std::remove(outputFilename.c_str());
    }
  else
    {
      outputCheck.close();
    }

  std::ofstream outputCache{};
  outputCache.open(outputFilename);
  assert(outputCache.is_open());

  std::cout << "Finding best ratio...\n";
  // double ratio = findBestRatio(filename, 0.5);
  double ratio = 0.01;

  if(centroids <= 1 || centroids > MAX_CENTROIDS)
    {
      std::cout << "Finding optimal number of centroids...\n";
      centroids = findElbow(filename, ratio);
      std::cout << "Optimal number of centroids is " << centroids << "\n";
    }
  outputCache << ratio << "\n";
  outputCache << centroids << "\n";
  processVideoLoop(filename, ratio, centroids, outputCache);

  std::cout << "100%\n";
  outputCache.close();
}

void processVideoLoop(std::string filename, double ratio, int centroids,
		      std::ofstream &outputCache)
{
  
  cv::VideoCapture video{filename, cv::CAP_FFMPEG};
  assert(video.isOpened());
  
  if(ratio <= 0 || ratio > 1)
    {
      std::cerr << "Invalid ratio. Defaulting to 0.08\n";
      ratio = 0.08;
    }
  
  int index{0};
  int frames(video.get(cv::CAP_PROP_FRAME_COUNT));
  cv::Mat frame{};
  cv::Mat resizedFrame{};
  int last{-1};
  std::string videoOutput{"_output.avi"};
  cv::VideoWriter videoWriter(filename + videoOutput, cv::VideoWriter::fourcc
			      ('M', 'J', 'P', 'G'), video.get(cv::CAP_PROP_FPS),
			      cv::Size(640, 480));

  assert(videoWriter.isOpened());
  
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
      processFrame(&resizedFrame, centroids, outputCache, videoWriter);

      double percentage = floor(static_cast<double>(static_cast<double>(index) /
						    static_cast<double>(frames)) * 100);
      if(percentage > last)
	{
	  last = static_cast<int>(percentage);
	  std::cout << percentage << "%\n";
	}
    }
  videoWriter.release();
  video.release();
}

void processFrame(cv::Mat* frame, int centroids, std::ofstream &outputCache,
		  cv::VideoWriter &videoWriter)
{
  if(centroids < 3 || centroids > MAX_CENTROIDS)
    {
      std::cout << centroids;
      std::cout << " is an invalid number. Defaulting to 3\n";
      centroids = 3;
    }

  std::vector<Pixel> pixelVector{};
  scanImage(*frame, pixelVector);
  std::vector<Centroid> centroidVector{};
  createAndProcessCentroids(pixelVector, centroidVector, centroids, 20);

  std::vector<int> ratioVector{};
  std::vector<Pixel> framePixelVector{};
  for(auto &centroid : centroidVector)
    {
      centroid.printLocation(true, outputCache, frame->rows * frame->cols);
      ratioVector.push_back((static_cast<double>(centroid.getOwnedPixels_ptrSize()) /
			     static_cast<double>(frame->rows * frame->cols)) * 100);
      framePixelVector.push_back(Pixel{centroid.getLocation().r, centroid.getLocation().g,
				       centroid.getLocation().b});
    }


  cv::Mat createdFrame{};
  createFrame(640, 480, framePixelVector, ratioVector,
	      videoWriter, createdFrame);
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
  double smallestLargestDifference{};
  int ratioOfSmallestLargestDifference{};
  double averageProcessTimeOfSmallestLargestDifference{};
  const int minRatio{1};
  const int maxRatio{15};

  std::vector<Color> colorVectorLarge{};
  extractColor(filename, percent, maxRatio, totalProcessTimeLarge,
	       averageProcessTimeLarge, colorVectorLarge);

  for (int ratio{minRatio}; ratio < maxRatio; ++ratio)
    {
      std::vector<Color> colorVectorSmall{};
      extractColor(filename, percent, ratio, totalProcessTimeSmall,
		   averageProcessTimeSmall, colorVectorSmall);

      compareAccuracy(colorVectorLarge, colorVectorSmall, largestDifference);


      if(largestDifference < 1.5)
	{
	  return static_cast<double>(static_cast<double>(ratio) / 100.0);
	}

      else if(ratio == minRatio)
	{
	  smallestLargestDifference = largestDifference;
	  ratioOfSmallestLargestDifference = ratio;
	  averageProcessTimeOfSmallestLargestDifference = averageProcessTimeSmall;
	}
      else if(smallestLargestDifference > largestDifference)
	{
	  smallestLargestDifference = largestDifference;
	  ratioOfSmallestLargestDifference = ratio;
	  averageProcessTimeOfSmallestLargestDifference = averageProcessTimeSmall;
	}

    }
  cv::VideoCapture video(filename, cv::CAP_FFMPEG);
  assert(video.isOpened());
  double frames{video.get(cv::CAP_PROP_FRAME_COUNT)};

  std::cout << "Picked ratio: " << static_cast<double>(static_cast<double>(ratioOfSmallestLargestDifference) / 100.0) << "\n";
  std::cout << "The largest difference with this ratio is: " << smallestLargestDifference << "\n";
  std::cout << "Each frame takes an average of: " << averageProcessTimeOfSmallestLargestDifference << "seconds\n";
  std::cout << "This video should take about: " << averageProcessTimeOfSmallestLargestDifference * frames << " seconds.\n";


  return static_cast<double>(static_cast<double>(ratioOfSmallestLargestDifference) / 100.0);
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
  int counter{0};
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
	      std::cout << percentComplete << "% complete with ratio " << ratio << "\n";
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

void compareAccuracy(std::vector<Color> colorVectorLarge,
		     std::vector<Color> colorVectorSmall,
		     double &largestDifference_o)
{
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
	}
    }
}

void createFrame(int width, int height, std::vector<Pixel> &pixelVector,
		 std::vector<int> &ratios, cv::VideoWriter &videoWriter, cv::Mat &createdFrame)
{
  createdFrame = cv::Mat::zeros(480, 640, CV_8UC3);
  int total{0};
  for(auto &ratio : ratios)
    {
      total += ratio;
    }

  if(total != 100)
    {
      ratios[ratios.size() - 1] += 100 - total;
    }

  drawFrame(width, height, pixelVector, ratios, createdFrame);
  writeFrame(videoWriter, createdFrame);
}

void drawFrame(int width, int height, std::vector<Pixel> &pixelVector,
	       std::vector<int> &ratios, cv::Mat &frame)
{
  int total{0};
  int totalWidthDrawn{0};
  assert(pixelVector.size() == ratios.size());
  reorder(pixelVector, ratios);
  assert(descending(ratios));
  for(std::size_t i{0}; i < pixelVector.size(); ++i)
    {
      const cv::Scalar tempScalar{static_cast<double>(pixelVector[i].b),
	static_cast<double>(pixelVector[i].g),
	static_cast<double>(pixelVector[i].r)};
      int drawWidth{static_cast<int>(floor(static_cast<double>(ratios[i] / 100.0) * width))};

      totalWidthDrawn += drawWidth;
      if(i == pixelVector.size() - 1)
	{
	  if(totalWidthDrawn != width)
	    {
	      int extra = width - totalWidthDrawn;
	      drawWidth += extra;
	      totalWidthDrawn += extra;
	    }
	}

      cv::Rect rectangleToDraw{total, 0, drawWidth,
	height};
      cv::rectangle(frame, rectangleToDraw, tempScalar, cv::FILLED);
      total += drawWidth + 1;
    }
}

void writeFrame(cv::VideoWriter &videoWriter, cv::Mat &frame)
{
  assert(videoWriter.isOpened());
  videoWriter.write(frame);
}

void reorder(std::vector<Pixel> &pixelVector, std::vector<int> &intVector)
{
  bool noSwap{false};
  while(!noSwap)
    {
      noSwap = true;
      for(std::size_t i{0}; i < pixelVector.size() - 1; ++i)
	{
	  if(intVector[i] < intVector[i + 1])
	    {
	      std::swap(intVector[i], intVector[i + 1]);
	      std::swap(pixelVector[i], pixelVector[i + 1]);
	      noSwap = false;
	    }
	}
    }
}

bool descending(std::vector<int> &intVector)
{
  for(std::size_t i{0}; i < intVector.size() - 1; ++i)
    {
      if(intVector[i] < intVector[i + 1])
	{
	  return false;
	}
    }
  return true;
}
void playVideos(std::string original, std::string color)
{
  cv::VideoCapture originalVideo(original, cv::CAP_FFMPEG);
  cv::VideoCapture colorVideo(color, cv::CAP_FFMPEG);

  double originalFPS{originalVideo.get(cv::CAP_PROP_FPS)};

  while(1)
    {
      cv::Mat originalFrame{};
      cv::Mat colorFrame{};

      originalVideo >> originalFrame;
      colorVideo >> colorFrame;

      if(colorFrame.empty() || originalFrame.empty())
	{
	  break;
	}

      cv::imshow("Original", originalFrame);
      cv::imshow("Color", colorFrame);

      if(cv::waitKey(1000 / originalFPS) == 27)
	{
	  break;
	}
    }
}

bool checkVideos(std::string original, std::string color, double tolerance)
{
  cv::VideoCapture originalVideo(original, cv::CAP_FFMPEG);
  cv::VideoCapture colorVideo(color, cv::CAP_FFMPEG);
  tolerance = round(tolerance);

  double originalFrames{originalVideo.get(cv::CAP_PROP_FRAME_COUNT)};
  double colorFrames{colorVideo.get(cv::CAP_PROP_FRAME_COUNT)};

  return (abs(originalFrames - colorFrames) <= 10);
}


bool checkCache(std::string filename)
{
  int lines{0};
  std::string cacheFile{filename + "_cache"};
  std::ifstream cacheFileStream{cacheFile};
  assert(cacheFileStream.is_open());

  std::string line{};
  while(std::getline(cacheFileStream, line))
    {
      lines++;
    }
  cacheFileStream.close();

  cv::VideoCapture videoThing(filename, cv::CAP_FFMPEG);
  double frames{videoThing.get(cv::CAP_PROP_FRAME_COUNT)};

  return frames == (lines - 2);
}
