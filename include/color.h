#ifndef COLOR_H
#define COLOR_H

#include "sRGB.h"
#include "XYZ.h"
#include "Lab.h"
#include "Lch.h"

class Color;


// This class contains four members with different color spaces
// One of each of the four color spaces defined above
// When an object of class Color is instantiated, it is done
// By passing in a triplet representing an sRGB color
// The color is immediately converted into XYZ, CIEL*ab, and CIEL*ch
// color spaces and stored as private members of the Color.
// Access is done through two function calls
// For example:
// Color color1{255, 193, 204} // This is a beautiful bubblegum color
// std::cout << color1.lab().a() << "\n"; // outputs 23.7715
// For more information on these color spaces, go to https://www.easyrgb.com/en/
class Color
{
private:
  void sRGBtoXYZConversion();
  void XYZtoCIELabConversion();
  void CIELabtoCIELchConversion();
  sRGB m_rgb;
  XYZ m_xyz;
  Lab m_lab;
  Lch m_lch;
public:
  Color();
  Color(double r, double g, double b);

  sRGB rgb();
  XYZ xyz();
  Lab lab();
  Lch lch();

  void outputSelf();
};


#endif


