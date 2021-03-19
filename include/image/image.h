#ifndef IMAGE_H
#define IMAGE_H

#include "../header-only/pixel.h"

#include <opencv2/opencv.hpp>

#include <string>


// Takes a reference to an image in cv::Mat form
// and a reference to a vector of pixels
// Clears whatever was already in the vector
// and fills is with pixel objects (pixel.h)
// Each pixel is assigned a unique ID here
void scanImage(cv::Mat &image, std::vector<Pixel> &pixelVector);

#endif
