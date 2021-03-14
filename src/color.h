#ifndef COLOR_H
#define COLOR_H

class Color;

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
  Color(double r, double g, double b);

  sRGB rgb();
  XYZ xyz();
  Lab lab();
  Lch lch();
};

double deltaE00Difference(Color color1, Color color2);

#endif


