#ifndef VIDEOPROCESS_H
#define VIDEOPROCESS_H

#include "../src/centroid.h"
#include "../src/pixel.h"

#include <opencv2/opencv.hpp>

#include <string>

void processVideo(std::string filename, int centroids);
void processFrame(cv::Mat *frame, int centroids, int currentFrame);
int findElbow(std::string filename);
int findElbowFrame(cv::Mat* frame);
double findRatio(int targetPixels, int minPixels, double ratio, double width, double height);

#endif
