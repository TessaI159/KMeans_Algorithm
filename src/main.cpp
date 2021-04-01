#include "videoProcess.h"
#include "constants.h"

#include <opencv2/opencv.hpp>

#include <fstream>



// TODO
// Intelligent auto number of centroids using elbow method - COMPLETED
// Auto optimal resize - COMPLETED
// Automatic conversion of video into compatible format/codec with ffmpeg library
// Implement unique pointers to ensure each pixel in the vector only has one pointer to it (C++11)
// Everything but GUI needs to be in C++ -- GOOD HERE
// Guesstimate how long the process will take, give or take a few seconds -- BORKED
// Cache partially finished results so we don't have to start over every single time
// or if something kills the process
// Differentiate between partial results and a fully processed video

// Find a good way to calculate the optimal # of frames to sample for
// elbow and accuracy tests (double percent)

// findBestRatio
// Remove the early stop and force the function to
// analyze every different ratio and base its decision off
// more than just color accuracy


int main(int argc, char* args[])
{
  std::string assets = "assets/";
  std::string filename{PROJECT_PATH + assets};
  std::string colorFilename = filename;

  if(argc == 2)
    {
      std::string fileAddition = args[1];
      filename += fileAddition;
      colorFilename += fileAddition;
      colorFilename += "_output.avi";
    }
  else if(argc == 1)
    {
      processVideo(filename, -1);
    }
  else
    {
      std::cerr << "executable video-name\n";
      return -1;
    }

  if(FILE *file = fopen(colorFilename.c_str(), "r"))
    {
      fclose(file);
      std::cout << "Looks like this one's already been processed. Want to process it again?";
      std::cout << " [y/n] ";
      char choice{};
      std::cin >> choice;
      if(choice == 'y')
	{
	  processVideo(filename, -1);
	}
    }
  else
    {
      processVideo(filename, -1);
    }



  checkVideos(filename, colorFilename, 3);
  playVideos(filename, colorFilename);

  return 0;
}


// For reference

// #include <chrono>
// using std::chrono::high_resolution_clock;
// using std::chrono::duration_cast;
// using std::chrono::duration;
// using std::chrono::milliseconds;
// auto startTime = high_resolution_clock::now();
// auto endTime = high_resolution_clock::now();

// duration<double, std::milli> processTime = duration_cast<milliseconds>
//        (endTime - startTime);
// double processTimeDouble{processTime.count() / 1000};
