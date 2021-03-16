#ifndef VIDEOPROCESS_H
#define VIDEOPROCESS_H

#include "../src/centroid.h"
#include "../src/pixel.h"
#include "../src/color.h"

#include <opencv2/opencv.hpp>

#include <string>
#include <map>

void processVideo(std::string filename, int centroids);
void processVideoLoop(std::string filename, double ratio, int centroids);
void processFrame(cv::Mat *frame, int centroids, int currentFrame);

int findElbow(std::string filename, double ratio);
std::map<int, int> findElbowLoop(std::string filename, double percent, double ratio);
int findElbowFrame(cv::Mat* frame);

double findBestRatio(std::string filename, double percent);
std::vector<Color> extractColor(std::string filename, double percent, double ratio, double &totalProcessTime_o, double &averageProcessTime_o);
std::vector<Color> extractColorFrame(cv::Mat *frame);
double compareAccuracy(std::vector<Color> largerColorVector, std::vector<Color> smallerColorVector, double &largestDistance_o);


#endif
