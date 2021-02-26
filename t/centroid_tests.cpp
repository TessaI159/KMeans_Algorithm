#include "../src/centroid.h"
#include "../src/pixel.h"
#define BOOST_TEST_MODULE CentroidTests
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

#include <vector>
#include <iostream>

BOOST_AUTO_TEST_SUITE(centroid_test_suite)

BOOST_AUTO_TEST_CASE(centroid_construction)
{
  Centroid centroid{};
  BOOST_CHECK_EQUAL(centroid.getID(), -1);
  BOOST_CHECK_EQUAL(centroid.getLocation().r, 0);
  BOOST_CHECK_EQUAL(centroid.getLocation().g, 0);
  BOOST_CHECK_EQUAL(centroid.getLocation().b, 0);
}

BOOST_AUTO_TEST_CASE(centroid_location)
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

  std::vector<Pixel *> pixelVector_ptr{&p0,&p1,&p2,&p3,&p4,&p5,&p6,&p7,&p8,&p9,&p10,&p11,&p12,&p13,&p14,&p15,&p16,&p17,&p18,&p19};

  // R average 132.7
  // G average 127.2
  // B average 123.65
}

BOOST_AUTO_TEST_SUITE_END()
