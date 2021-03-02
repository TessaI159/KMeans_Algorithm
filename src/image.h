#ifndef IMAGE_H
#define IMAGE_H

#include "pixel.h"

#include <opencv2/opencv.hpp>

#include <string>

const std::vector<Pixel> scanImage(cv::Mat &image);

#endif
