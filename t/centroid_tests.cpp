#include "../src/centroid.h"
#include "../src/pixel.h"
#include "../src/kmeans.h"
#define BOOST_TEST_MODULE CentroidTests
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

#include <vector>
#include <iostream>

BOOST_AUTO_TEST_SUITE(centroid_test_suite)

BOOST_AUTO_TEST_CASE(centroid_construction)
{
  std::cout << "Centroid tests\n";
  std::cout << "\tconstruction\n";
  Centroid centroid{};
  BOOST_CHECK_EQUAL(centroid.getID(), -1);
  BOOST_CHECK_EQUAL(centroid.getLocation().r, 0);
  BOOST_CHECK_EQUAL(centroid.getLocation().g, 0);
  BOOST_CHECK_EQUAL(centroid.getLocation().b, 0);

  Pixel p0{158,15,14};
  Pixel p1{186,139,77};
  Pixel p2{210,220,26};
  Pixel p3{98,49,185};
  Pixel p4{156,220,44};
  Pixel p5{83,108,2};
  Pixel p6{120,154,199};
  Pixel p7{248,157,114};
  Pixel p8{86,145,198};
  Pixel p9{85,237,63};
  Pixel p10{88,99,29};
  Pixel p11{65,98,189};
  Pixel p12{99,207,193};
  Pixel p13{212,76,247};
  Pixel p14{41,17,137};
  Pixel p15{222,40,233};
  Pixel p16{36,40,82};
  Pixel p17{108,250,110};
  Pixel p18{235,74,86};
  Pixel p19{118,199,245};

  std::vector<Pixel> pixelVector{p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12
				 ,p13,p14,p15,p16,p17,p18,p19};

  std::vector<Pixel *> pixelVector_ptr{&p0,&p1,&p2,&p3,&p4,&p5,&p6,&p7,&p8,&p9,&p10,&p11,&p12
				       ,&p13,&p14,&p15,&p16,&p17,&p18,&p19};
  Centroid centroid_2(pixelVector_ptr);
  std::vector<Pixel *> pixelVectorInner{centroid_2.getOwnedPixels_ptr()};

  for(std::size_t i{0}; i < pixelVector_ptr.size(); ++i)
    {
      BOOST_CHECK_EQUAL(pixelVector_ptr[i]->r, pixelVectorInner[i]->r);
      BOOST_CHECK_EQUAL(pixelVector_ptr[i]->g, pixelVectorInner[i]->g);
      BOOST_CHECK_EQUAL(pixelVector_ptr[i]->b, pixelVectorInner[i]->b);
      BOOST_CHECK_EQUAL(pixelVector_ptr[i]->id, pixelVectorInner[i]->id);
      BOOST_CHECK_EQUAL(pixelVector_ptr[i]->ownedBy, pixelVectorInner[i]->ownedBy);
      BOOST_CHECK_EQUAL(pixelVector_ptr[i]->swapTo, pixelVectorInner[i]->swapTo);
      BOOST_CHECK_EQUAL(pixelVector_ptr[i]->needsSwapped, pixelVectorInner[i]->needsSwapped);
    }
}

BOOST_AUTO_TEST_CASE(centroid_location)
{
  std::cout << "\tlocation\n";
  Pixel p0{158,15,14};
  Pixel p1{186,139,77};
  Pixel p2{210,220,26};
  Pixel p3{98,49,185};
  Pixel p4{156,220,44};
  Pixel p5{83,108,2};
  Pixel p6{120,154,199};
  Pixel p7{248,157,114};
  Pixel p8{86,145,198};
  Pixel p9{85,237,63};
  Pixel p10{88,99,29};
  Pixel p11{65,98,189};
  Pixel p12{99,207,193};
  Pixel p13{212,76,247};
  Pixel p14{41,17,137};
  Pixel p15{222,40,233};
  Pixel p16{36,40,82};
  Pixel p17{108,250,110};
  Pixel p18{235,74,86};
  Pixel p19{118,199,245};

  std::vector<Pixel> pixelVector{p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12
				 ,p13,p14,p15,p16,p17,p18,p19};

  std::vector<Pixel *> pixelVector_ptr{&p0,&p1,&p2,&p3,&p4,&p5,&p6,&p7,&p8,&p9,&p10,&p11,&p12
				       ,&p13,&p14,&p15,&p16,&p17,&p18,&p19};
  Centroid centroid(pixelVector_ptr);

  BOOST_CHECK_EQUAL(centroid.getLocation().r, 132);
  BOOST_CHECK_EQUAL(centroid.getLocation().g, 127);
  BOOST_CHECK_EQUAL(centroid.getLocation().b, 123);
}

BOOST_AUTO_TEST_CASE(centroid_distance, * boost::unit_test::tolerance(0.001))
{
  std::cout << "\tdistance\n";
  Pixel p0{158,15,14};
  Pixel p1{186,139,77};
  Pixel p2{210,220,26};
  Pixel p3{98,49,185};
  Pixel p4{156,220,44};
  Pixel p5{83,108,2};
  Pixel p6{120,154,199};
  Pixel p7{248,157,114};
  Pixel p8{86,145,198};
  Pixel p9{85,237,63};
  Pixel p10{88,99,29};
  Pixel p11{65,98,189};
  Pixel p12{99,207,193};
  Pixel p13{212,76,247};
  Pixel p14{41,17,137};
  Pixel p15{222,40,233};
  Pixel p16{36,40,82};
  Pixel p17{108,250,110};
  Pixel p18{235,74,86};
  Pixel p19{118,199,245};

  std::vector<Pixel> pixelVector{p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12
				 ,p13,p14,p15,p16,p17,p18,p19};

  std::vector<Pixel *> pixelVector_ptr{&p0,&p1,&p2,&p3,&p4,&p5,&p6,&p7,&p8,&p9,&p10,&p11,&p12
				       ,&p13,&p14,&p15,&p16,&p17,&p18,&p19};
  Centroid centroid(pixelVector_ptr);

  // R average 132
  // G average 127
  // B average 123

  Pixel p20{173,204,75}; // 9914.000099
  Pixel p21{101,77,59}; // 7556.999978
  Pixel p22{39,21,17}; // 31121.00004

  BOOST_TEST(centroid.distanceFromPixel(&p20) == 9914.000099);
  BOOST_TEST(centroid.distanceFromPixel(&p21) == 7556.999978);
  BOOST_TEST(centroid.distanceFromPixel(&p22) == 31121.00004);
}

BOOST_AUTO_TEST_CASE(centroid_add_release)
{
  std::cout << "\tadd/release\n";
  Pixel p0{158,15,14,0};
  Pixel p1{186,139,77,1};
  Pixel p2{210,220,26,2};
  Pixel p3{98,49,185,3};
  Pixel p4{156,220,44,4};
  Pixel p5{83,108,2,5};
  Pixel p6{120,154,199,6};
  Pixel p7{248,157,114,7};
  Pixel p8{86,145,198,8};
  Pixel p9{85,237,63,9};
  Pixel p10{88,99,29,10};
  Pixel p11{65,98,189,11};
  Pixel p12{99,207,193,12};
  Pixel p13{212,76,247,13};
  Pixel p14{41,17,137,14};
  Pixel p15{222,40,233,15};
  Pixel p16{36,40,82,16};
  Pixel p17{108,250,110,17};
  Pixel p18{235,74,86,18};
  Pixel p19{118,199,245,19};

  std::vector<Pixel> pixelVector{p0,p1,p2,p3,p4,p5,p6,p7,p8,p9};
  std::vector<Pixel> pixelVector_2{p10,p11,p12,p13,p14,p15,p16,p17,p18,p19};

  std::vector<Pixel *> pixelVector_ptr{&p0,&p1,&p2,&p3,&p4,&p5,&p6,&p7,&p8,&p9};
  std::vector<Pixel *> pixelVector_ptr_2{&p10,&p11,&p12,&p13,&p14,&p15,&p16,&p17,&p18,&p19};

  Centroid centroid{pixelVector_ptr};
  Centroid centroid_2{pixelVector_ptr_2};

  BOOST_CHECK_EQUAL(centroid.getOwnedPixels_ptr().size(), 10);
  BOOST_CHECK_EQUAL(centroid_2.getOwnedPixels_ptr().size(), 10);

  centroid_2.addPixel(centroid.releasePixel(4));
  centroid.addPixel(centroid_2.releasePixel(17));

  bool centroid_correct = false;
  bool centroid_2_correct = false;

  for(auto pixel : centroid.getOwnedPixels_ptr())
    {
      if(pixel->id == 17)
	{
	  centroid_correct = true;
	}
    }

  for(auto pixel : centroid_2.getOwnedPixels_ptr())
    {
      if(pixel->id == 4)
	{
	  centroid_2_correct = true;
	}
    }

  BOOST_TEST(centroid_correct);
  BOOST_TEST(centroid_2_correct);
  BOOST_CHECK_EQUAL(centroid_2.releasePixel(17), nullptr);

}

BOOST_AUTO_TEST_CASE(centroid_getters)
{
  Pixel p0{158,15,14};
  Pixel p1{186,139,77};
  Pixel p2{210,220,26};
  Pixel p3{98,49,185};
  Pixel p4{156,220,44};
  Pixel p5{83,108,2};
  Pixel p6{120,154,199};
  Pixel p7{248,157,114};
  Pixel p8{86,145,198};
  Pixel p9{85,237,63};
  Pixel p10{88,99,29};
  Pixel p11{65,98,189};
  Pixel p12{99,207,193};
  Pixel p13{212,76,247};
  Pixel p14{41,17,137};
  Pixel p15{222,40,233};
  Pixel p16{36,40,82};
  Pixel p17{108,250,110};
  Pixel p18{235,74,86};
  Pixel p19{118,199,245};

  std::vector<Pixel> pixelVector{p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12
				 ,p13,p14,p15,p16,p17,p18,p19};
  
  std::vector<Centroid> centroidVector{createCentroids(pixelVector, 2)};

  
  BOOST_CHECK_EQUAL(centroidVector[0].getPixelByID(-1), nullptr);
  BOOST_CHECK_EQUAL(centroidVector[0].getPixelByID(10), nullptr);
  BOOST_CHECK_EQUAL(centroidVector[0].getPixelByID(15), nullptr);
  BOOST_CHECK_EQUAL(centroidVector[1].getPixelByID(-1), nullptr);
  BOOST_CHECK_EQUAL(centroidVector[1].getPixelByID(0), nullptr);
  BOOST_CHECK_EQUAL(centroidVector[1].getPixelByID(4), nullptr);
}

BOOST_AUTO_TEST_SUITE_END()
