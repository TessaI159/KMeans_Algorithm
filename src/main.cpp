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



int main(int argc, char* args[])
{
  bool statistics{true};
  int targetPixels{3000};
  int minPixels{800};
  std::string filename{"/home/tess/Code/KMeans/assets/"};

  if(argc == 2)
    {
      std::string fileAddition = args[1];
      filename += fileAddition;
    }
  else if(argc == 4)
    {
      targetPixels = atoi(args[1]);
      minPixels = atoi(args[2]);
      std::string fileAddition = args[3];
      filename += fileAddition;
    }
  else
    {
      std::cerr << "You forgot something...\n";
      return -1;
    }

  std::cout << "Target pixels: " << targetPixels << "\n";
  std::cout << "Min pixels: " << minPixels << "\n";
  std::cout << "Filename: " << filename << "\n";


  if (statistics)
    {
      using std::chrono::high_resolution_clock;
      using std::chrono::duration_cast;
      using std::chrono::duration;
      using std::chrono::milliseconds;
      
      auto startTime = high_resolution_clock::now();
      processVideo(filename, -1, targetPixels, minPixels);
      auto endTime = high_resolution_clock::now();

      duration<double, std::milli> processTime = duration_cast<milliseconds>(endTime - startTime);

      cv::VideoCapture video(filename, cv::CAP_FFMPEG);
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
      processVideo(filename, -1, targetPixels, minPixels);
    }


  return 0;
}
