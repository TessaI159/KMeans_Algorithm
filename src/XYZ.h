#ifndef XYZ_H
#define XYZ_H

class Color;

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

#endif
