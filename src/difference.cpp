#include "difference.h"

#include <math.h>

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
