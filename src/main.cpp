#include "pixel.h"
#include "image.h"
#include "centroid.h"

#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>


int main()
{
  cv::Mat loadedImage = cv::imread("/home/tess/Code/KMeans/assets/test_image.png");
  return 0;
}
