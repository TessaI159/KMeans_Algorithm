#ifndef COLOR_H
#define COLOR_H

class Color;

// A standard sRGB color space
class sRGB
{
private:
  double m_r;
  double m_g;
  double m_b;
public:
  sRGB();
  double r();
  double g();
  double b();
  friend class Color;
};

// XYZ color space
class XYZ
{
private:
  double m_x;
  double m_y;
  double m_z;
public:
  XYZ();
  double x();
  double y();
  double z();

  friend class Color;
};


// CIEL*ab color space
class Lab
{
private:
  double m_l;
  double m_a;
  double m_b;
public:
  Lab();
  double l();
  double a();
  double b();

  friend class Color;
};


// CIEL*ch color space
class Lch
{
private:
  double m_l;
  double m_c;
  double m_h;
public:
  Lch();
  double l();
  double c();
  double h();

  friend class Color;
};


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
};


// Difference and Conversion functions provided by https://www.easyrgb.com/en/math.php
// Finds the difference between the two colors using the deltaE 2000 method by CIE
// Note: this function is quasimetric
// You may get a different result by plugging in (x, y) than you would get plugging
// in (y, x). I recommend passing the "reference" color first, meaning the color that
// you would consider the more "accurate" color
double deltaE00Difference(Color color1, Color color2);

#endif


