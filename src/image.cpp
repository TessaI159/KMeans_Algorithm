#include "image.h"
#include "pixel.h"

#include <vector>

#include <opencv2/opencv.hpp>

const std::vector<Pixel> scanImage(cv::Mat &image)
{
  std::vector<Pixel> pixelVector{};
  int channels = image.channels();
  int id{-1};

  int rows = image.rows;
  int cols = image.cols * channels;

  if(image.isContinuous())
    {
      cols *= rows;
      rows = 1;
    }

  uchar* ptr{nullptr};
  for(int i{0}; i < rows; ++i)
    {
      ptr = image.ptr<uchar>(i);
      Pixel tempPixel{};
      for(int j{0}; j < cols; j += 3)
	{
	  // Opencv uses BGR format by default
	  tempPixel.b = ptr[j];
	  tempPixel.g = ptr[j + 1];
	  tempPixel.r = ptr[j + 2];
	  tempPixel.id = ++id;
	  pixelVector.push_back(tempPixel);
	}
    }
  const std::vector<Pixel> pixelVector_c{pixelVector};
  return pixelVector_c;
}
