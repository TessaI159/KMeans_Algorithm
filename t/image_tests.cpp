#include "../src/centroid.h"
#include "../src/pixel.h"
#include "../src/image.h"
#define BOOST_TEST_MODULE CentroidTests
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

#include <vector>
#include <iostream>

BOOST_AUTO_TEST_SUITE(image_tests)

BOOST_AUTO_TEST_CASE(image_loading)
{
  std::cout << "Image tests\n";
  std::cout << "\tloading\n";

  cv::Mat image{loadImage("/home/tess/Code/KMeans/assets/test_image.png")};

  BOOST_TEST(image.data);
}

BOOST_AUTO_TEST_SUITE_END()
