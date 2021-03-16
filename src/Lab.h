#ifndef LAB_H
#define LAB_H

class Color;

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

#endif
