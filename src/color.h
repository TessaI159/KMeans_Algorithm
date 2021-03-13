#ifndef COLOR_H
#define COLOR_H

class sRGB
{
private:
  double m_r;
  double m_g;
  double m_b;
public:
  sRGB();
  sRGB(double r, double g, double b);
  double getr();
  double getg();
  double getb();
  void setr(double r);
  void setg(double g);
  void setb(double b);
};

class XYZ
{
private:
  double m_x;
  double m_y;
  double m_z;
public:
  XYZ();
  XYZ(double x, double y, double z);
  double getx();
  double gety();
  double getz();
};

class Lab
{
private:
  double m_l;
  double m_a;
  double m_b;
public:
  Lab();
  Lab(double l, double a, double b);
  double getl();
  double geta();
  double getb();
};

class Lch
{
private:
  double m_l;
  double m_c;
  double m_h;
public:
  Lch();
  Lch(double l, double c, double h);
  double getl();
  double getc();
  double geth();
};

class Color
{
private:
  void sRGBtoXYZConversion();
  void XYZtoCIELabConversion();
  void CIELabtoCIELchConversion();
public:
  Color(double r, double g, double b);
  sRGB rgb;
  XYZ xyz;
  Lab lab;
  Lch lch;
};

#endif


