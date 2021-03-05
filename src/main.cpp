#include "videoProcess.h"

// TODO
// Intelligent auto number of centroids using elbow method
// Auto optimal resize - COMPLETED
// Automatic conversion of video into compatible format/codec with ffmpeg library
// Implement unique pointers to ensure each pixel in the vector only has one pointer to it
// Figure out default variables for a forward-declared function
// Class constructor initialization lists
// New, delete, smart pointers


// Distortion: It is calculated as the average of the squared distances from the
// cluster centers of the respective clusters. Typically, the Euclidean distance metric is used.
// Inertia: It is the sum of squared distances of samples to their closest cluster center.

// For the elbow test use distortion, current point calculation, and 2..10 centroids inclusive


int main()
{
  processVideo("/home/tess/Code/KMeans/assets/color.avi");
  return 0;
}
