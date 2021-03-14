#include "color.h"
#include "constants.h"

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
double Lab::b() { return m_b; }


Lch::Lch(){}

double Lch::l() { return m_l; }
double Lch::c() { return m_c; }
double Lch::h() { return m_h; }


Color::Color(double r, double g, double b)
{
  m_rgb.m_r = r;
  m_rgb.m_g = g;
  m_rgb.m_b = b;
  sRGBtoXYZConversion();
  XYZtoCIELabConversion();
  CIELabtoCIELchConversion();
}

void Color::sRGBtoXYZConversion()
{
  double vR {m_rgb.m_r / 255.0};

  if(vR > 0.04045)
    {
      vR = pow(((vR + 0.055) / 1.055), 2.4);
    }
  else
    {
      vR /= 12.92;
    }

  double vG {m_rgb.m_g / 255.0};

  if(vG > 0.04045)
    {
      vG = pow(((vG + 0.055) / 1.055), 2.4);
    }
  else
    {
      vG /= 12.92;
    }

  double vB {m_rgb.m_b / 255.0};

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
  double vX {m_xyz.m_x / X_2};
  if(vX > 0.008856)
    {
      vX = pow(vX, (1.0/3.0));
    }
  else
    {
      vX = (7.787 * vX) + (16.0 / 116.0);
    }

  double vY {m_xyz.m_y / Y_2};
  if(vY > 0.008856)
    {
      vY = pow(vY, (1.0/3.0));
    }
  else
    {
      vY = (7.787 * vY) + (16.0 / 116.0);
    }

  double vZ {m_xyz.m_z / Z_2};
  if(vZ > 0.008856)
    {
      vZ = pow(vZ, (1.0/3.0));
    }
  else
    {
      vZ = (7.787 * vZ) + (16.0 / 116.0);
    }

  m_lab.m_l = (116.0 * vY) - 16;
  m_lab.m_a = 500.0 * (vX - vY);
  std::cout << m_lab.m_a << "\n";
  m_lab.m_b = 200.0 * (vY - vZ);
  std::cout << m_lab.m_b << "\n";
}

void Color::CIELabtoCIELchConversion()
{
  double vH {atan2(m_lab.m_b, m_lab.m_a)};

  if(vH > 0)
    {
      vH = (vH / PI) * 180;
    }
  else
    {
      vH = 360 - (abs(vH) / PI) * 180;
    }

  m_lch.m_l = m_lab.m_l;
  m_lch.m_c = sqrt(pow(m_lab.m_a, 2) + pow(m_lab.m_b, 2));
  m_lch.m_h = vH;
}

sRGB Color::rgb(){ return m_rgb; }
XYZ Color::xyz(){ return m_xyz; }
Lab Color::lab(){ return m_lab; }
Lch Color::lch(){ return m_lch; }


double deltaE00Difference(Color color1, Color color2)
{
  // For later use
  double hTick_1{};
  double hTick_2{};
  double deltahTick{};
  double deltaHTick{};
  double HBarTick{};
  // For later use

  double K_L {1.0};
  double K_C {1.0};
  double K_H {1.0};
  double deltaLTick {color2.lab().l() - color1.lab().l()};
  double LBarTick {(color1.lab().l() - color2.lab().l()) / 2};
  double CBar {(color1.lch().c() + color2.lch().c()) / 2};
  double aTick_1 {color1.lab().a() + ((color1.lab().a() / 2) * (1 - sqrt(pow(CBar, 7) / (pow(CBar, 7) + pow(25, 7)))))};
  double aTick_2 {color2.lab().a() + ((color1.lab().a() / 2) * (1 - sqrt(pow(CBar, 7) / (pow(CBar, 7) + pow(25, 7)))))};

  double CTick_1 {sqrt(pow(aTick_1, 2) + pow(color1.lab().b(), 2))};
  double CTick_2 {sqrt(pow(aTick_2, 2) + pow(color2.lab().b(), 2))};
  double CBarTick {(CTick_1 + CTick_2) / 2};
  double deltaCTick {CTick_2 - CTick_1};

  if(aTick_1 == 0 && color1.lab().b() == 0)
    {
      hTick_1 = 0;
    }
  else
    {
      hTick_1 = fmod(atan2(color1.lab().b(), aTick_1), 360.0);
    }

  if(aTick_2 == 0 && color2.lab().b() == 0)
    {
      hTick_2 = 0;
    }
  else
    {
      hTick_2 = fmod(atan2(color2.lab().b(), aTick_2), 360.0);
    }

  if(CTick_1 == 0 || CTick_2 == 0)
    {
      deltahTick = 0;
    }
  else
    {
      if(abs(hTick_1 - hTick_2) <= 180)
	{
	  deltahTick = hTick_2 - hTick_1;
	}
      else if(abs(hTick_1 - hTick_2) > 180 && hTick_2 <= hTick_1)
	{
	  deltahTick = hTick_2 - hTick_1 + 360;
	}
      else if(abs(hTick_1 - hTick_2) > 180 && hTick_2 > hTick_1)
	{
	  deltahTick = hTick_2 - hTick_1 - 360;
	}
    }
  deltaHTick = 2 * sqrt(CTick_1 * CTick_2) * sin(deltahTick / 2);

  if(CTick_1 == 0 || CTick_2 == 0)
    {
      HBarTick = hTick_1 + hTick_2;
    }
  else {
    if(abs(hTick_1 - hTick_2) <= 180)
      {
	HBarTick = (hTick_1 + hTick_2) / 2;
      }
    else if(abs(hTick_1 - hTick_2) > 180 && hTick_1 + hTick_2 < 360)
      {
	HBarTick = (hTick_1 + hTick_2 + 360) / 2;
      }
    else if(abs(hTick_1 + hTick_2) > 180 && hTick_1 + hTick_2 >= 360)
      {
	HBarTick = (hTick_1 + hTick_2 - 360) / 2;
      }
  }

  double T = 1 - 0.17 * cos(HBarTick - 30) + 0.24 * cos(2 * HBarTick) + 0.32 * cos(3 * HBarTick + 6) - 0.2 * cos(4 * HBarTick - 63);
  double S_L = 1 + ((0.015 * pow((LBarTick - 50), 2) / sqrt(20 + pow((LBarTick - 50), 2))));
  double S_C = 1 + 0.045 * CBarTick;
  double S_H = 1 + 0.015 * CBarTick * T;
  double R_T = -2 * sqrt(pow(CBarTick, 7) / (pow(CBarTick, 7) + pow(25, 7))) * sin(-(((pow(((HBarTick - 275) / 25), 2)))));

  double arg_L = deltaLTick / (K_L * S_L);
  double arg_C = deltaCTick / (K_C * S_C);
  double arg_H = deltaHTick / (K_H * S_H);

  return sqrt(pow(arg_L, 2) + pow(arg_C, 2) + pow(arg_H, 2) + (R_T * arg_C * arg_H));
}
