#include "videoProcess.h"

// TODO
// Intelligent auto number of centroids using elbow method
// Auto optimal resize - COMPLETED
// Automatic conversion of video into compatible format/codec with ffmpeg library
// Implement unique pointers to ensure each pixel in the vector only has one pointer to it
// Figure out default variables for a forward-declared function
// Class constructor initialization lists
// New, delete, smart pointers
// Trim any pure blck frames off the beginning of the video


// Distortion: It is calculated as the average of the squared distances from the
// cluster centers of the respective clusters. Typically, the Euclidean distance metric is used.

// For the elbow test use distortion, current point calculation, and 2..10 centroids inclusive


int main()
{
  processVideo("/home/tess/Code/KMeans/assets/nemo.avi", -1);
  return 0;
}
