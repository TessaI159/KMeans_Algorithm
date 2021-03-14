#ifndef VIDEOPROCESS_H
#define VIDEOPROCESS_H

#include "../src/centroid.h"
#include "../src/pixel.h"
#include "../src/color.h"

#include <opencv2/opencv.hpp>

#include <string>
#include <map>

void processVideo(std::string filename, int centroids,
		  int targetPixels, int minPixels);
void processVideoLoop(std::string filename, double ratio, int centroids);
void processFrame(cv::Mat *frame, int centroids, int currentFrame);

int findElbow(std::string filename, int targetPixels, int minPixels);
std::map<int, int> findElbowLoop(std::string filename, double percent, double ratio);
int findElbowFrame(cv::Mat* frame);

double findBestRatio(std::string filename, double percent);
std::vector<Color> processRatio(std::string filename, double percent, double raio);
Color processRatioFrame(cv::Mat *frame);



// To be deleted
double findRatio(int targetPixels, int minPixels, double ratio, double width, double height);

#endif
