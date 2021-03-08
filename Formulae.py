#############
# sRGB -> XYZ
#############

# // sR, sG and sB (Standard RGB) input range = 0 ÷ 255
# // X, Y and Z output refer to a D65/2° standard illuminant.

# var_R = ( sR / 255 )
# var_G = ( sG / 255 )
# var_B = ( sB / 255 )

# if ( var_R > 0.04045 ) var_R = ( ( var_R + 0.055 ) / 1.055 ) ^ 2.4
# else                   var_R = var_R / 12.92
# if ( var_G > 0.04045 ) var_G = ( ( var_G + 0.055 ) / 1.055 ) ^ 2.4
# else                   var_G = var_G / 12.92
# if ( var_B > 0.04045 ) var_B = ( ( var_B + 0.055 ) / 1.055 ) ^ 2.4
# else                   var_B = var_B / 12.92

# var_R = var_R * 100
# var_G = var_G * 100
# var_B = var_B * 100

# X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805
# Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722
# Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505

# From http://www.easyrgb.com/en/math.php

#################
# XYZ -> CIE L*ab
#################

# // Reference-X, Y and Z refer to specific illuminants and observers.
# // Common reference values are available below in this same page.

# var_X = X / Reference-X
# var_Y = Y / Reference-Y
# var_Z = Z / Reference-Z

# if ( var_X > 0.008856 ) var_X = var_X ^ ( 1/3 )
# else                    var_X = ( 7.787 * var_X ) + ( 16 / 116 )
# if ( var_Y > 0.008856 ) var_Y = var_Y ^ ( 1/3 )
# else                    var_Y = ( 7.787 * var_Y ) + ( 16 / 116 )
# if ( var_Z > 0.008856 ) var_Z = var_Z ^ ( 1/3 )
# else                    var_Z = ( 7.787 * var_Z ) + ( 16 / 116 )

# CIE-L* = ( 116 * var_Y ) - 16
# CIE-a* = 500 * ( var_X - var_Y )
# CIE-b* = 200 * ( var_Y - var_Z )

#################
# CIE L*ab -> XYZ
#################

# // Reference-X, Y and Z refer to specific illuminants and observers.
# // Common reference values are available below in this same page.

# var_Y = ( CIE-L* + 16 ) / 116
# var_X = CIE-a* / 500 + var_Y
# var_Z = var_Y - CIE-b* / 200

# if ( var_Y^3  > 0.008856 ) var_Y = var_Y^3
# else                       var_Y = ( var_Y - 16 / 116 ) / 7.787
# if ( var_X^3  > 0.008856 ) var_X = var_X^3
# else                       var_X = ( var_X - 16 / 116 ) / 7.787
# if ( var_Z^3  > 0.008856 ) var_Z = var_Z^3
# else                       var_Z = ( var_Z - 16 / 116 ) / 7.787

# X = var_X * Reference-X
# Y = var_Y * Reference-Y
# Z = var_Z * Reference-Z

############
# XYZ -> RGB
############

# // X, Y and Z input refer to a D65/2° standard illuminant.
# // sR, sG and sB (standard RGB) output range = 0 ÷ 255

# var_X = X / 100
# var_Y = Y / 100
# var_Z = Z / 100

# var_R = var_X *  3.2406 + var_Y * -1.5372 + var_Z * -0.4986
# var_G = var_X * -0.9689 + var_Y *  1.8758 + var_Z *  0.0415
# var_B = var_X *  0.0557 + var_Y * -0.2040 + var_Z *  1.0570

# if ( var_R > 0.0031308 ) var_R = 1.055 * ( var_R ^ ( 1 / 2.4 ) ) - 0.055
# else                     var_R = 12.92 * var_R
# if ( var_G > 0.0031308 ) var_G = 1.055 * ( var_G ^ ( 1 / 2.4 ) ) - 0.055
# else                     var_G = 12.92 * var_G
# if ( var_B > 0.0031308 ) var_B = 1.055 * ( var_B ^ ( 1 / 2.4 ) ) - 0.055
# else                     var_B = 12.92 * var_B

# sR = var_R * 255
# sG = var_G * 255
# sB = var_B * 255

###############
# L*ab -> L*CH°
###############

# var_H = arc_tangent( CIE-b*, CIE-a* )  //Quadrant by signs

# if ( var_H > 0 ) var_H = ( var_H / PI ) * 180
# else             var_H = 360 - ( abs( var_H ) / PI ) * 180

# CIE-L* = CIE-L*
# CIE-C* = sqrt( CIE-a* ^ 2 + CIE-b* ^ 2 )
# CIE-H° = var_H

###############
# L*CH° -> L*ab
###############

# CIE-H° range = 0 ÷ 360°

# CIE-L* = CIE-L*
# CIE-a* = cos( degree_2_radian( CIE-H° ) ) * CIE-C*
# CIE-b* = sin( degree_2_radian( CIE-H° ) ) * CIE-C*

#################
# D65/2° Tristimulus

# 2° CIE 1931
# X_2°: 95.047
# Y_2°: 100.000
# Z_2°: 108.883
#################
