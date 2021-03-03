#include "../src/centroid.h"
#include "../src/pixel.h"
#include "../src/image.h"
#define BOOST_TEST_MODULE ImageTests
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

#include <vector>
#include <iostream>
#include <time.h>
#include <stdlib.h>

BOOST_AUTO_TEST_SUITE(image_tests)

BOOST_AUTO_TEST_CASE(image_loading)
{
  std::cout << "Image tests\n";
  std::cout << "\tloading\n";
  srand(time(0));
  
  cv::Mat image{cv::imread("/home/tess/Code/KMeans/assets/cooldownpink.jpg")};
  std::vector<Pixel> pixelVector{scanImage(image)};
  int pixelVectorSize = pixelVector.size();

  BOOST_REQUIRE(image.data);
  
  for(int i{0}; i < pixelVectorSize / 50; ++i)
    {
      int randomNumber = rand();
      BOOST_CHECK_EQUAL(pixelVector[randomNumber % pixelVectorSize].r, 255);
      BOOST_CHECK_EQUAL(pixelVector[randomNumber % pixelVectorSize].g, 144);
      BOOST_CHECK_EQUAL(pixelVector[randomNumber % pixelVectorSize].b, 176);
      BOOST_TEST(pixelVector[randomNumber % pixelVectorSize].id != -1);
    }
}

BOOST_AUTO_TEST_SUITE_END()
