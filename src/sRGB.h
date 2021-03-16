#ifndef SRGB_H
#define SRGB_H

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

#endif
