#ifndef DIFFERENCE_H
#define DIFFERENCE_H

#include "color.h"

// Math shamelessly taken from https://en.wikipedia.org/wiki/Color_difference
// Finds the difference between the two colors using the deltaE 2000 method by CIE
// Note: this function is quasimetric
// You may get a different result by plugging in (x, y) than you would get plugging
// in (y, x). I recommend passing the "reference" color first, meaning the color that
// you would consider the more "accurate" color
double deltaE00Difference(Color color1, Color color2);

#endif
