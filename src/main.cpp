#include "videoProcess.h"

#include <opencv2/opencv.hpp>

// TODO
// Intelligent auto number of centroids using elbow method
// Auto optimal resize - COMPLETED
// Automatic conversion of video into compatible format/codec with ffmpeg library
// Implement unique pointers to ensure each pixel in the vector only has one pointer to it
// Figure out default variables for a forward-declared function
// Class constructor initialization lists
// New, delete, smart pointers
// Trim any pure blck frames off the beginning of the video


// Distortion: It is calculated as the average of the squared distances from the
// cluster centers of the respective clusters. Typically, the Euclidean distance metric is used.

// For the elbow test use distortion, current point calculation, and 2..10 centroids inclusive


int main()
{
  bool statistics{true};
  if (statistics)
    {
      std::clock_t startTime{clock()};
      processVideo("/home/tess/Code/KMeans/assets/color2.avi", -1);
      std::clock_t endTime{clock()};

      double processTime{static_cast<double>((endTime - startTime) / CLOCKS_PER_SEC)};
      cv::VideoCapture video("/home/tess/Code/KMeans/assets/color2.avi", cv::CAP_FFMPEG);
      assert(video.isOpened());
      double videoLength{video.get(cv::CAP_PROP_FRAME_COUNT) / video.get(cv::CAP_PROP_FPS)};
      double averageTimePerSecond{processTime / videoLength};
      double averageTimePerFrame{processTime / video.get(cv::CAP_PROP_FRAME_COUNT)};
      std::cout << "Process took: " << processTime << " seconds for a video that is ";
      std::cout << videoLength << " seconds long.\n";
      std::cout << "There was an average processing time of " << averageTimePerSecond << " seconds per 1 second of video, ";
      std::cout << " which indicates an average processing time of " << averageTimePerFrame << " seconds per frame.\n";
    }
  else
    {
      processVideo("/home/tess/Code/KMeans/assets/color2.avi", -1);
    }


  return 0;
}
