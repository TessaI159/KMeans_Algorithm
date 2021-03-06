#include "videoProcess.h"

#include <opencv2/opencv.hpp>

#include <chrono>

// TODO
// Intelligent auto number of centroids using elbow method - COMPLETED
// Auto optimal resize - COMPLETED
// Automatic conversion of video into compatible format/codec with ffmpeg library
// Implement unique pointers to ensure each pixel in the vector only has one pointer to it
// Figure out default variables for a forward-declared function
// Class constructor initialization lists
// Smart pointers
// Trim any pure blck frames off the beginning and end of the video



int main()
{



  bool statistics{true};
  if (statistics)
    {
      using std::chrono::high_resolution_clock;
      using std::chrono::duration_cast;
      using std::chrono::duration;
      using std::chrono::milliseconds;
      
      auto startTime = high_resolution_clock::now();
      processVideo("/home/tess/Code/KMeans/assets/color2.avi", -1);
      auto endTime = high_resolution_clock::now();

      duration<double, std::milli> processTime = duration_cast<milliseconds>(endTime - startTime);

      cv::VideoCapture video("/home/tess/Code/KMeans/assets/color2.avi", cv::CAP_FFMPEG);
      assert(video.isOpened());

      double frames{video.get(cv::CAP_PROP_FRAME_COUNT)};
      double fps{video.get(cv::CAP_PROP_FPS)};
      double totalVideoDuration{frames / fps};
      double processTimeDouble{processTime.count() / 1000};

      std::cout << "Process took a total of " << processTimeDouble << " seconds\n";
      std::cout << "Process took a total of " << processTimeDouble / totalVideoDuration
		<< " seconds per second of video\n";
      std::cout << "Process took a total of " << processTimeDouble / frames
		<< " seconds per frame\n";
    }
  else
    {
      processVideo("/home/tess/Code/KMeans/assets/color2.avi", -1);
    }


  return 0;
}
