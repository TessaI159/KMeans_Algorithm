#include "color.h"

#include <iostream>
#include <math.h>

sRGB::sRGB(){}

double sRGB::r() { return m_r; }
double sRGB::g() { return m_g; }
double sRGB::b() { return m_b; }


XYZ::XYZ(){}

double XYZ::x() { return m_x; }
double XYZ::y() { return m_y; }
double XYZ::z() { return m_z; }


Lab::Lab(){}

double Lab::l() { return m_l; }
double Lab::a() { return m_a; }
double Lab::b() { return m_a; }


Lch::Lch(){}

double Lch::l() { return m_l; }
double Lch::c() { return m_c; }
double Lch::h() { return m_c; }


Color::Color(double r, double g, double b)
{
  m_rgb.m_r = r;
  m_rgb.m_g = g;
  m_rgb.m_b = b;
  sRGBtoXYZConversion();
}

void Color::sRGBtoXYZConversion()
{
  double vR = m_rgb.m_r / 255.0;
  
  if(vR > 0.04045)
    {
      vR = pow(((vR + 0.055) / 1.055), 2.4);
    }
  else
    {
      vR /= 12.92;
    }

  double vG = m_rgb.m_g / 255.0;

  if(vG > 0.04045)
    {
      vG = pow(((vG + 0.055) / 1.055), 2.4);
    }
  else
    {
      vG /= 12.92;
    }

  double vB = m_rgb.m_b / 255.0;

  if(vB > 0.04045)
    {
      vB = pow(((vB + 0.055) / 1.055), 2.4);
    }
  else
    {
      vB /= 12.92;
    }

  vR *= 100;
  vG *= 100;
  vB *= 100;

  m_xyz.m_x = (vR * 0.4124) + (vG * 0.3576) + (vB * 0.1805);
  m_xyz.m_y = (vR * 0.2126) + (vG * 0.7152) + (vB * 0.0722);
  m_xyz.m_z = (vR * 0.0193) + (vG * 0.1192) + (vB * 0.9505);
}

void Color::XYZtoCIELabConversion()
{
  
}

void Color::CIELabtoCIELchConversion()
{
  
}

sRGB Color::rgb(){ return m_rgb; }
XYZ Color::xyz(){ return m_xyz; }
Lab Color::lab(){ return m_lab; }
Lch Color::lch(){ return m_lch; }
