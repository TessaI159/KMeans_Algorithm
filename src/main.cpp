#include "videoProcess.h"
#include "constants.h"
#include "color.h"

#include <opencv2/opencv.hpp>

#include <chrono>
#include <fstream>

// TODO
// Intelligent auto number of centroids using elbow method - COMPLETED
// Auto optimal resize - COMPLETED
// Automatic conversion of video into compatible format/codec with ffmpeg library
// Implement unique pointers to ensure each pixel in the vector only has one pointer to it
// Smart pointers
// Trim any pure blck frames off the beginning and end of the video

// Everything but GUI needs to be in C++
// Sample ~2% of frames to test for color accuracy/speed
// Guesstimate how long the process will take, give or take a few seconds


int main(int argc, char* args[])
{
  bool statistics{true};
  std::string assets = "assets/";
  std::string filename{PROJECT_PATH + assets};

  if (statistics)
    {
      if(argc == 2)
	{
	  std::string fileAddition = args[1];
	  filename += fileAddition;
	}
      else
	{
	  std::cerr << "Script videoName\n";
	  return -1;
	}

      using std::chrono::high_resolution_clock;
      using std::chrono::duration_cast;
      using std::chrono::duration;
      using std::chrono::milliseconds;
      auto startTime = high_resolution_clock::now();
      processVideo(filename, -1);
      auto endTime = high_resolution_clock::now();

      duration<double, std::milli> processTime = duration_cast<milliseconds>(endTime - startTime);

      cv::VideoCapture video(filename, cv::CAP_FFMPEG);
      assert(video.isOpened());

      // double frames{video.get(cv::CAP_PROP_FRAME_COUNT)};
      // double fps{video.get(cv::CAP_PROP_FPS)};
      // double totalVideoDuration{frames / fps};
      double processTimeDouble{processTime.count() / 1000};

      std::ofstream output{};
      output.open(PROJECT_PATH + "output", std::ios_base::app);
      if(output.fail())
	{
	  std::cerr << "Could not open output file in main\n";
	  std::cerr << filename << "\n";
	  return -1;
	}
      else
	{
	  output << processTimeDouble  << "\n";
	}
      output.close();

    }
  else
    {
      processVideo(filename, -1);
    }

  return 0;
}
