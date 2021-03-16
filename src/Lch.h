#ifndef LCH_H
#define LCH_H

class Color;

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

#endif
