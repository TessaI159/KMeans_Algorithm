#include "color.h"

sRGB::sRGB(){}
sRGB::sRGB(double r, double g, double b) : m_r{r}, m_g{g}, m_b{b}
{

}

double sRGB::getr() { return m_r; }
double sRGB::getg() { return m_g; }
double sRGB::getb() { return m_b; }
void sRGB::setr(double r) { m_r = r; }
void sRGB::setg(double g) { m_g = g; }
void sRGB::setb(double b) { m_b = b; }


XYZ::XYZ(){}
XYZ::XYZ(double x, double y, double z) : m_x{x}, m_y{y}, m_z{z}
{

}

double XYZ::getx() { return m_x; }
double XYZ::gety() { return m_y; }
double XYZ::getz() { return m_z; }

Lab::Lab(){}
Lab::Lab(double l, double a, double b) : m_l{l}, m_a{a}, m_b{b}
{

}

double Lab::getl() { return m_l; }
double Lab::geta() { return m_a; }
double Lab::getb() { return m_a; }

Lch::Lch(){}
Lch::Lch(double l, double c, double h) : m_l{l}, m_c{c}, m_h{h}
{

}

double Lch::getl() { return m_l; }
double Lch::getc() { return m_c; }
double Lch::geth() { return m_c; }

Color::Color(double r, double g, double b)
{
  rgb.setr(r);
  rgb.setg(g);
  rgb.setb(b);
}
