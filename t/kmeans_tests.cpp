#include "../src/centroid.h"
#include "../src/pixel.h"
#include "../src/image.h"
#include "../src/kmeans.h"
#define BOOST_TEST_MODULE CentroidTests
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <opencv2/opencv.hpp>

#include <vector>
#include <iostream>

BOOST_AUTO_TEST_SUITE(kmeans_test_suite)

BOOST_AUTO_TEST_CASE(centroid_vector_creation)
{
  std::cout << "KMeans tests\n";
  std::cout << "\tcentroid vector creation\n";
  cv::Mat image{cv::imread("/home/tess/Code/KMeans/assets/cooldownpink.jpg")};
  // 76800 pixels total
  const std::vector<Pixel> pixelVector{scanImage(image)};

  BOOST_REQUIRE(image.data);

  std::vector<Centroid> centroidVector{createCentroids(pixelVector, 3)};

  for(auto centroid : centroidVector)
    {
      BOOST_CHECK_EQUAL(centroid.getOwnedPixels_ptr().size(), 25600);
    }
}

BOOST_AUTO_TEST_SUITE_END()
