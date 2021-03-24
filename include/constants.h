#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

constexpr int MAX_CENTROIDS{6};
constexpr int TARGET_PIXELS{1800};
constexpr int MIN_PIXELS{800};
const std::string PROJECT_PATH {"/home/tess/Code/VHS/"};

// These are the CIE tristimulus x, y, and z reference values with a D65
// observer illuminant at 2°
constexpr double X_2{95.047};
constexpr double Y_2{100.00};
constexpr double Z_2{108.883};

#endif
