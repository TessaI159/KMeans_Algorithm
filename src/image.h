#ifndef IMAGE_H
#define IMAGE_H

#include "pixel.h"

#include <opencv2/opencv.hpp>

#include <string>


cv::Mat loadImage(std::string filename);
std::vector<Pixel> scanImage(cv::Mat &image);

#endif
