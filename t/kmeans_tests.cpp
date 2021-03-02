#include "../src/centroid.h"
#include "../src/pixel.h"
#include "../src/image.h"
#include "../src/kmeans.h"
#define BOOST_TEST_MODULE KMeansTests
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
  std::vector<Pixel> pixelVector{scanImage(image)};

  BOOST_REQUIRE(image.data);

  std::vector<Centroid> centroidVector{createCentroids(pixelVector, 3)};

  for(auto centroid : centroidVector)
    {
      BOOST_CHECK_EQUAL(centroid.getOwnedPixels_ptr().size(), 25600);
    }
  for(std::size_t i{0}; i < centroidVector.size(); ++i)
    {
      BOOST_CHECK_EQUAL(centroidVector[i].getID(), i);
      std::vector<Pixel *> loopPixelVector_ptr{centroidVector[i].getOwnedPixels_ptr()};
      std::size_t loopPixelVector_ptrSize{loopPixelVector_ptr.size()};
      for(std::size_t j{0}; j < loopPixelVector_ptrSize; ++j)
	{
	  BOOST_CHECK_EQUAL(loopPixelVector_ptr[j]->id, (i * loopPixelVector_ptrSize) + j);
	}
    }
}

BOOST_AUTO_TEST_CASE(smallest_element)
{
  std::cout << "\tsmallest element\n";
  double test1[] {37.0, 14.0, 0.0, 2.0};
  double test2[] {41.0, 57.0, 22.0, 94.0, 1.0};
  double test3[] {-1.0, -16.0, -47.0, 19.0};
  double test4[] {47000.0, 57213.0, 5432.0, 84315.0, 14.1, 14.2};
  double test5[] {14.3,64.8,75.5,76.7,10.1,77.1,72.6,20.7,70.2,60.7,89.5,8.9,4.4,79.2,18.8,45.3,87.6,76.1,11.5,7.6,15,13,51.3,19.8,24.9,98.7,75.6,6.8,39.6,31.4,78.3,11.4,11.8,9,45.1,45.9,76.6,92,63.9,58.1,60.6,14.2,44.4,59.1,72.4,82.1,85,5.2,57.7,18.2};
  double test6[] {47871.7,26731.3,20618.4,33494.5,32722.2,23890.5,15162.1,15648.2,10719.8,31799.9,13303.5,48824.1,25287.9,30324.6,11021.7,24741.8,48484.4,16011.2,15686.2,18249.1,43444.8,17001.5,19055.4,11949.5,17126.6,26720.4,24750.1,26630.3,13514.8,41353.2,32726.6,20588.2,48980.5,23018.3,43624.3,38368.5,19362.4,24996,15970.1,40569.7,22130.1,18194.8,10045.6,17239.8,28614.8,45358.6,38256.6,35016.9,16626,45704.6};
  double test7[] {1.03191,1.94775,1.45948,1.18732,1.77569,1.57197,1.76605,1.85556,1.65968,1.44816,1.01398,1.63394,1.48984,1.23909,1.54536,1.27995,1.52074,1.22198,1.87632,1.34806,1.73744,1.65636,1.67193,1.1143,1.0927,1.27263,1.62226,1.0454,1.28065,1.73849,1.31569,1.41324,1.23112,1.66912,1.20875,1.1547,1.5593,1.16984,1.06562,1.36092,1.41874,1.52714,1.0524,1.66438,1.25979,1.02768,1.21289,1.61486,1.35874,1.10346};

  // 2, 4, 2, 4

  BOOST_CHECK_EQUAL(smallestElement(test1, 4), 2);
  BOOST_CHECK_EQUAL(smallestElement(test2, 5), 4);
  BOOST_CHECK_EQUAL(smallestElement(test3, 4), 2);
  BOOST_CHECK_EQUAL(smallestElement(test4, 6), 4);
  BOOST_CHECK_EQUAL(smallestElement(test5, 50), 12);
  BOOST_CHECK_EQUAL(smallestElement(test6, 50), 42);
  BOOST_CHECK_EQUAL(smallestElement(test7, 50), 10);
}

BOOST_AUTO_TEST_CASE(pixel_swap)
{
  std::cout << "\tpixel swap\n";

  std::vector<Pixel> pixelVector{};

  pixelVector.push_back(Pixel{163,245,147, 0});
  pixelVector.push_back(Pixel{92,55,93, 1});
  pixelVector.push_back(Pixel{239,16,1, 2});
  pixelVector.push_back(Pixel{177,24,116, 3});

  std::vector<Centroid> centroidVector{createCentroids(pixelVector, 2)};

  setSwap(&pixelVector[0], 1);
  setSwap(&pixelVector[1], 1);
  setSwap(&pixelVector[2], 0);
  setSwap(&pixelVector[3], 0);

  BOOST_CHECK_EQUAL(pixelVector[0].needsSwapped, true);
  BOOST_CHECK_EQUAL(pixelVector[1].needsSwapped, true);
  BOOST_CHECK_EQUAL(pixelVector[2].needsSwapped, true);
  BOOST_CHECK_EQUAL(pixelVector[3].needsSwapped, true);
  BOOST_CHECK_EQUAL(pixelVector[0].swapTo, 1);
  BOOST_CHECK_EQUAL(pixelVector[1].swapTo, 1);
  BOOST_CHECK_EQUAL(pixelVector[2].swapTo, 0);
  BOOST_CHECK_EQUAL(pixelVector[3].swapTo, 0);

  runSwaps(centroidVector, pixelVector);

  BOOST_CHECK_EQUAL(pixelVector[0].needsSwapped, false);
  BOOST_CHECK_EQUAL(pixelVector[1].needsSwapped, false);
  BOOST_CHECK_EQUAL(pixelVector[2].needsSwapped, false);
  BOOST_CHECK_EQUAL(pixelVector[3].needsSwapped, false);
  BOOST_CHECK_EQUAL(pixelVector[0].swapTo, -1);
  BOOST_CHECK_EQUAL(pixelVector[1].swapTo, -1);
  BOOST_CHECK_EQUAL(pixelVector[2].swapTo, -1);
  BOOST_CHECK_EQUAL(pixelVector[3].swapTo, -1);
}

BOOST_AUTO_TEST_CASE(centroid_updates)
{
  std::cout << "\tcentroid updates\n";
  std::vector<Pixel> pixelVector{};

  // 148, 184, 77
  pixelVector.push_back(Pixel{173,109,191});
  pixelVector.push_back(Pixel{38,132,50});
  pixelVector.push_back(Pixel{191,244,132});
  pixelVector.push_back(Pixel{211,163,31});
  pixelVector.push_back(Pixel{32,248,39});
  pixelVector.push_back(Pixel{196,254,140});
  pixelVector.push_back(Pixel{184,140,4});
  pixelVector.push_back(Pixel{113,206,69});
  pixelVector.push_back(Pixel{199,149,85});
  pixelVector.push_back(Pixel{146,204,38});

  // 161, 100, 123
  pixelVector.push_back(Pixel{244,103,79});
  pixelVector.push_back(Pixel{99,76,230});
  pixelVector.push_back(Pixel{106,107,252});
  pixelVector.push_back(Pixel{197,166,76});
  pixelVector.push_back(Pixel{170,68,150});
  pixelVector.push_back(Pixel{214,119,244});
  pixelVector.push_back(Pixel{227,180,64});
  pixelVector.push_back(Pixel{37,6,16});
  pixelVector.push_back(Pixel{138,63,113});
  pixelVector.push_back(Pixel{182,116,7});

  // Swaps
  // 0, 13, 16, 19

  for(std::size_t i{0}; i < pixelVector.size(); ++i)
    {
      pixelVector[i].id = i;
    }
  std::vector<Centroid> centroidVector{createCentroids(pixelVector, 2)};
  
  while(updateCentroidOwnership(centroidVector, pixelVector)){}

  BOOST_CHECK_EQUAL(pixelVector[0].ownedBy, 1);
  BOOST_CHECK_EQUAL(pixelVector[13].ownedBy, 0);
  BOOST_CHECK_EQUAL(pixelVector[16].ownedBy, 0);
  BOOST_CHECK_EQUAL(pixelVector[19].ownedBy, 0);

  std::cout << "ID\t\tPixel\t\tCentroid ID\t\tCentroid Location\n";

  for(auto &pixel : pixelVector)
    {
      std::cout << pixel.id << "\t\t";
      std::cout << pixel.r << ", " << pixel.g << ", " << pixel.b << "\t\t";
      std::cout << pixel.ownedBy << "\t\t";
      std::cout << centroidVector[pixel.ownedBy].getLocation().r << ", ";
      std::cout << centroidVector[pixel.ownedBy].getLocation().g << ", ";
      std::cout << centroidVector[pixel.ownedBy].getLocation().b;
      std::cout << "\n";
    }
}

BOOST_AUTO_TEST_SUITE_END()
