import os
import sys
import math
from pathlib import Path

class Centroid:
    r = 0
    g = 0
    b = 0
    ratio = 0
    frame = 0
    index = 0


class sRGB:
    sR = 0.0
    sG = 0.0
    sB = 0.0

    def __init__(self, sR, sG, sB):
        self.sR = sR
        self.sG = sG
        self.sB = sB


class XYZ:
    x = 0.0
    y = 0.0
    z = 0.0

    def __init__(self):
        self.x = 0.0
        self.y = 0.0
        self.z = 0.0


class Lab:
    l = 0.0
    a = 0.0
    b = 0.0

    def __init__(self):
        self.l = 0.0
        self.a = 0.0
        self.b = 0.0


class Lch:
    l = 0.0
    c = 0.0
    h = 0.0

    def __init__(self):
        self.l = 0.0
        self.c = 0.0
        self.h = 0.0


class Color:
    def __init__(self, r, g, b):
        self.rgb = sRGB(r, g, b)
        self.xyz = XYZ()
        self.lab = Lab()
        self.lch = Lch()
        self._X_2 = 95.047
        self._Y_2 = 100.00
        self._Z_2 = 108.883
        self.__sRGBtoXYZConversion()
        self.__XYZtoCIELabConversion()
        self.__CIELabtoCIELchConversion()


    def __sRGBtoXYZConversion(self):
        # X Y and Z use a standard  D65/2° illuminant

        self.rgb._vR = self.rgb.sR / 255.0
        self.rgb._vG = self.rgb.sG / 255.0
        self.rgb._vB = self.rgb.sB / 255.0

        if(self.rgb._vR > 0.04045):
            self.rgb._vR = ((self.rgb._vR + 0.055) / 1.055)**2.4
        else:
            self.rgb._vR /= 12.92

        if(self.rgb._vG > 0.04045):
            self.rgb._vG = ((self.rgb._vG + 0.055) / 1.055)**2.4
        else:
            self.rgb._vG /= 12.92

        if(self.rgb._vB > 0.04045):
            self.rgb._vB = ((self.rgb._vB + 0.055) / 1.055)**2.4
        else:
            self.rgb._vB /= 12.92

        self.rgb._vR *= 100
        self.rgb._vG *= 100
        self.rgb._vB *= 100

        self.xyz.x = (self.rgb._vR * 0.4124) + (self.rgb._vG * 0.3576) + (self.rgb._vB * 0.1805)
        self.xyz.y = (self.rgb._vR * 0.2126) + (self.rgb._vG * 0.7152) + (self.rgb._vB * 0.0722)
        self.xyz.z = (self.rgb._vR * 0.0193) + (self.rgb._vG * 0.1192) + (self.rgb._vB * 0.9505)


    def __XYZtoCIELabConversion(self):
        # _X_2, _Y_2, and _Z_2 use a D65/2° standard illuminant

        self.xyz._vX = self.xyz.x / self._X_2
        self.xyz._vY = self.xyz.y / self._Y_2
        self.xyz._vZ = self.xyz.z / self._Z_2

        if(self.xyz._vX > 0.008856):
            self.xyz._vX = self.xyz._vX**(1.0 / 3.0)
        else:
            self.xyz._vX = (7.787 * self.xyz._vX) + (16.0 / 116.0)

        if(self.xyz._vY > 0.008856):
            self.xyz._vY = self.xyz._vY**(1.0 / 3.0)
        else:
            self.xyz._vY = (7.787 * self.xyz._vY) + (16.0 / 116.0)

        if(self.xyz._vZ > 0.008856):
            self.xyz._vZ = self.xyz._vZ**(1.0 / 3.0)
        else:
            self.xyz._vZ = (7.787 * self.xyz._vZ) + (16.0 / 116.0)

        self.lab.l = (116.0 * self.xyz._vY) - 16
        self.lab.a = 500.0 * (self.xyz._vX - self.xyz._vY)
        self.lab.b = 200.0 * (self.xyz._vY - self.xyz._vZ)


    def __CIELabtoCIELchConversion(self):
        self.lch._vH = math.atan2(self.lab.b, self.lab.a)

        if(self.lch._vH > 0):
            self.lch._vH = (self.lch._vH / math.pi) * 180
        else:
            self.lch._vH = 360 - (abs(self.lch._vH) / math.pi) * 180

        self.lch.l = self.lab.l
        self.lch.c = math.sqrt((self.lab.a**2) + (self.lab.b**2))
        self.lch.h = self.lch._vH


def simpleEuclideanDifference(color1, color2):
    return math.sqrt(((color1.rgb.sR - color2.rgb.sR)**2) + ((color1.rgb.sG - color2.rgb.sG)**2) + ((color1.rgb.sB - color2.rgb.sB)**2))


def weightedEuclideanDifference(color1, color2):
    if(((color1.rgb.sR + color2.rgb.sR) / 2) < 128):
        r = 2.0
        b = 3.0
    else:
        r = 3.0
        b = 2.0
    return math.sqrt(((r * (color1.rgb.sR - color2.rgb.sR)**2) + (4.0 * (color1.rgb.sG - color2.rgb.sR)**2) + (b * (color1.rgb.sB - color2.rgb.sB)**2)))


def redmeanEuclideanDifference(color1, color2):
    rAverage = ((color1.rgb.sR + color2.rgb.sR) / 2)

    return math.sqrt(((2 + (rAverage / 256) * (color1.rgb.sR - color2.rgb.sR)**2) + ((4 * (color1.rgb.sG - color2.rgb.sG)**2)) + ((2 + ((255 - rAverage) / 256)) * (color1.rgb.sB - color2.rgb.sB)**2)))


def taxicabDifference(color1, color2):
    return math.sqrt(((color1.lab.a - color2.lab.a)**2) + ((color1.lab.b - color2.lab.b)**2)) + abs(color1.lab.l - color2.lab.l)


def deltaCDifference(color1, color2):
    return math.sqrt((color2.lab.a**2) + (color2.lab.b**2)) - math.sqrt((color1.lab.a**2) + (color1.lab.b**2))


def deltaHDifference(color1, color2):
    return math.sqrt(((color2.lab.a - color1.lab.a)**2) + ((color2.lab.b - color1.lab.b)**2) - ((deltaCDifference(color1, color2))**2))

# Tested: Working
def deltaE76Difference(color1, color2):
    return math.sqrt(((color1.lab.l - color2.lab.l)**2) + ((color1.lab.a - color2.lab.a)**2) + ((color1.lab.b - color2.lab.b)**2))

# Tested: Working
def deltaE94Difference(color1, color2):
    whtL = 1
    whtC = 1
    whtH = 1
    xC1 = math.sqrt((color1.lab.a**2) + (color1.lab.b**2))
    xC2 = math.sqrt((color2.lab.a**2) + (color2.lab.b**2))
    xDL = color2.lab.l - color1.lab.l
    xDC = xC2 - xC1
    xDE = deltaE76Difference(color1, color2)
    xDH = (xDE**2) - (xDL**2) - (xDC**2)

    if(xDH > 0):
        xDH = math.sqrt(xDH)
    else:
        xDH = 0

    xSC = 1 + (0.045 * xC1)
    xSH = 1 + (0.015 * xC1)
    xDL /= whtL
    xDC /= whtC * xSC
    xDH /= whtH * xSH

    return math.sqrt((xDL**2) + (xDC**2) + (xDH**2))

# Tested: Working
def deltaE00Difference(color1, color2):
    K_L = K_C = K_H = 1.0
    deltaLTick = color2.lab.l - color1.lab.l
    LBarTick = (color1.lab.l + color2.lab.l) / 2
    CBar = (color1.lch.c + color2.lch.c) / 2
    aTick_1 = color1.lab.a + (color1.lab.a / 2) * (1 - math.sqrt(CBar**7 / (CBar**7 + 25**7)))
    aTick_2 = color2.lab.a + (color2.lab.a / 2) * (1 - math.sqrt(CBar**7 / (CBar**7 + 25**7)))

    CTick_1 = math.sqrt(aTick_1**2 + color1.lab.b**2)
    CTick_2 = math.sqrt(aTick_2**2 + color2.lab.b**2)
    CBarTick = (CTick_1 + CTick_2) / 2
    deltaCTick = CTick_2 - CTick_1

    if(aTick_1 == 0 and color1.lab.b == 0):
        hTick_1 = 0
    else:
        hTick_1 = math.atan2(color1.lab.b, aTick_1) % 360

    if(aTick_2 == 0 and color2.lab.b == 0):
        hTick_2 = 0
    else:
        hTick_2 = math.atan2(color2.lab.b, aTick_2) % 360

    if(CTick_1 == 0 or CTick_2 == 0):
        deltahTick = 0
    else:
        if(abs(hTick_1 - hTick_2) <= 180):
            deltahTick = hTick_2 - hTick_1
        elif(abs(hTick_1 - hTick_2) > 180 and hTick_2 <= hTick_1):
            deltahTick = hTick_2 - hTick_1 + 360
        elif(abs(hTick_1 - hTick_2) > 180 and hTick_2 > hTick_1):
            deltahTick = hTick_2 - hTick_1 - 360

    deltaHTick = 2 * math.sqrt(CTick_1 * CTick_2) * math.sin(deltahTick / 2)

    if(CTick_1 == 0 or CTick_2 == 0):
        HBarTick = hTick_1 + hTick_2
    else:
        if(abs(hTick_1 - hTick_2) <= 180):
            HBarTick = (hTick_1 + hTick_2) / 2
        elif(abs(hTick_1 - hTick_2) > 180 and hTick_1 + hTick_2 < 360):
            HBarTick = (hTick_1 + hTick_2 + 360) / 2
        elif(abs(hTick_1 - hTick_2) > 180 and hTick_1 + hTick_2 >= 360):
            HBarTick = (hTick_1 + hTick_2 - 360) / 2

    T = 1 - 0.17 * math.cos(HBarTick - 30) + 0.24 * math.cos(2 * HBarTick) + 0.32 * math.cos(3 * HBarTick + 6) - 0.2 * math.cos(4 * HBarTick - 63)
    S_L = 1 + ((0.015 * (LBarTick - 50)**2) / math.sqrt(20 + (LBarTick - 50)**2))
    S_C = 1 + 0.045 * CBarTick
    S_H = 1 + 0.015 * CBarTick * T
    R_T = -2 * math.sqrt(CBarTick**7 / (CBarTick**7 + 25**7)) * math.sin(-(((HBarTick - 275) / 25)**2))

    arg_L = deltaLTick / (K_L * S_L)
    arg_C = deltaCTick / (K_C * S_C)
    arg_H = deltaHTick / (K_H * S_H)

    return math.sqrt(arg_L**2 + arg_C**2 + arg_H**2 + (R_T * arg_C * arg_H))


# Tested: Working
def deltaCMCDifference(color1, color2, lightness=2, chroma=1):
    if(color1.lch.l < 16):
        S_L = 0.511
    else:
        S_L = ((0.040975 * color1.lch.l) / (1 + 0.01765 * color1.lch.l))

    S_C = ((0.0638 * color1.lch.c) / (1 + 0.0131 * color1.lch.c)) + 0.638
    F = math.sqrt((color1.lch.c**4) / (color1.lch.h**4 + 1900))
    if(color1.lch.h >= 164 and color1.lch.h <= 345):
        T = 0.56 + abs(0.2 * math.cos(color1.lch.h + 168))
    else:
        T = 0.36 + abs(0.4 * math.cos(color1.lch.h + 35))

    S_H = S_C * (F * T + 1 - F)

    arg_L = ((color2.lch.l - color1.lch.l) / (lightness * S_L))
    arg_C = ((color2.lch.c - color1.lch.l) / (chroma * S_C))
    arg_H = ((deltaHDifference(color1, color2) / (S_H)))

    return math.sqrt((arg_L)**2 + (arg_C)**2 + (arg_H)**2)


def allDifferences(ref, color):
    print(f'Ref: ({ref.rgb.sR}, {ref.rgb.sG}, {ref.rgb.sB}) vs Color: ({color.rgb.sR}, {color.rgb.sG}, {color.rgb.sB})')
    print("Simple Euclidean: ", end="")
    print(simpleEuclideanDifference(ref, color))
    print("Weighted Euclidean: ", end="")
    print(weightedEuclideanDifference(ref, color))
    print("Redmean Euclidean: ", end="")
    print(redmeanEuclideanDifference(ref, color))
    print("Taxicab: ", end="")
    print(taxicabDifference(ref, color))
    print("Delta C: ", end="")
    print(deltaCDifference(ref, color))
    print("Delta H: ", end="")
    print(deltaHDifference(ref, color))
    print("Delta E 1976: ", end="")
    print(deltaE76Difference(ref, color))
    print("Delta E 1994: ", end="")
    print(deltaE94Difference(ref, color))
    print("Delta E 2000: ", end="")
    print(deltaE00Difference(ref, color))
    print("Delta CMC: ", end="")
    print(deltaCMCDifference(ref, color))
    print()

def createCentroidList(file):
    centroidList = []

    with open(file) as f:
        lines = [line.rstrip() for line in f]

    targetPixels = lines.pop(-1)
    executionTime = lines.pop(-1)

    for line in lines:
        if(len(line) == 0):
            lines.remove(line)

    for line in lines:
        if(line[0] == "F"):
            currentFrame = line[7:]
            continue
        else:
            centroidIndex = line[0]
            splitList = line[3:].split(', ')
            tempCentroid = Centroid()
            tempCentroid.frame = int(currentFrame)
            tempCentroid.index = int(centroidIndex)
            tempCentroid.r = int(splitList.pop(0))
            tempCentroid.g = int(splitList.pop(0))
            tempCentroid.b = int(splitList.pop(0))
            tempCentroid.ratio = float(splitList.pop(0).replace('with ', ''))
            centroidList.append(tempCentroid)

    for centroid in centroidList:
        if(centroid.r < 0):
            centroid.r = -1
        if(centroid.g < 0):
            centroid.g = -1
        if(centroid.b < 0):
            centroid.b = -1


    return (centroidList, targetPixels, executionTime)


def compareOutputs(centroidListSmall, centroidListLarge, executionSmall, executionLarge, pixelsSmall, pixelsLarge):
    totalDifference = 0
    totalCounted = 0
    executionDifference = float(executionLarge) / float(executionSmall) * 100.0
    
    for i in range(0, len(centroidListSmall)):
        tempColor = Color(centroidListSmall[i].r, centroidListSmall[i].g, centroidListSmall[i].b)
        refColor = Color(centroidListLarge[i].r, centroidListLarge[i].g, centroidListLarge[i].b)
        if(refColor.rgb.sR <= -1 or refColor.rgb.sG <= -1 or refColor.rgb.sB <= -1 or tempColor.rgb.sR <= -1 or tempColor.rgb.sG <= -1 or tempColor.rgb.sB <= -1):
            continue
        else:
            totalDifference += deltaE00Difference(refColor, tempColor)
            totalCounted += 1

        
    averageDifference = totalDifference / totalCounted
    print(f'Reducing from {pixelsLarge} to {pixelsSmall} pixels caused the script to run %.3f%%  as fast with an average loss of %.3f accuracy in color output' % (executionDifference, averageDifference))
    print('For reference, a change of ~2.3 represents a barely noticeable change in the colors, if you have the two side by side')
    print()
    
    
def largest(list):
    largestItem = list[0]
    largestIndex = 0
    for x in range(len(list)):
        if(list[x] > largestItem):
            largestIndex = x
            largestItem = list[x]

    return largestIndex


defaultMinPixels = 250
centroidListList = []
centroidListExecution = []



################################
############# MAIN #############
################################

# refColor = Color(50,168,82)
# colorChanged = Color(56,156,83)
# color1 = Color(44, 187, 51)
# color2 = Color(181, 59, 86)
# color3 = Color(252, 197, 23)
# color4 = Color(126, 195, 77)
# color5 = Color(195, 130, 200)
# color6 = Color(212, 7, 115)
# color7 = Color(82, 145, 36)
# color8 = Color(120, 250, 154)
# color9 = Color(250, 105, 224)
# color10 = Color(44, 181, 233)

# colorList = [color1, color2, color3, color4, color5, color6, color7, color8, color9, color10]

# for color in colorList:
#     allDifferences(refColor, color)

# allDifferences(refColor, colorChanged)

# print(f'Color 1 CIE-L*ab: {color1.lab.l}, {color1.lab.a}, {color1.lab.b}')
# print(f'Color 2 CIE-L*ab: {color2.lab.l}, {color2.lab.a}, {color2.lab.b}')


# if(len(sys.argv) < 2):
#     print("Must input video name")
#     exit()
# elif(len(sys.argv) == 2):
#     print("Using default pixels of 1200 and 3000")
#     pixelList = [1200, 3000]
#     videoName = sys.argv[1]
# else:
#     videoName = sys.argv[1]
#     pixelList = []
#     for pixel in sys.argv[2:]:
#         pixelList.append(int(pixel))

# largestPixelIndex = largest(pixelList)

# for pixel in pixelList:
#     pixelStr = str(pixel)
#     filePath = Path(f'/home/tess/Code/KMeans_algorithm/output{pixelStr}_{videoName[:-4]}')
#     if(filePath.is_file()):
#         print(f'{videoName} has already been run using {pixel} pixels as a parameter. Skipping.')
#     else:
#         os.system(f'./main.run {pixel} {defaultMinPixels} {videoName}')
#         os.system(f'mv output output{pixelStr}_{videoName[:-4]}')
        
#     centroidListOutput = createCentroidList(f'output{pixelStr}_{videoName[:-4]}')

#     centroidListList.append(centroidListOutput[0])
#     centroidListExecution.append(centroidListOutput[2])

# for x in range(len(centroidListList)):
#     if(x != largestPixelIndex):
#         compareOutputs(centroidListList[x], centroidListList[largestPixelIndex], centroidListExecution[x], centroidListExecution[largestPixelIndex], pixelList[x], pixelList[largestPixelIndex])

# script video [pixels pixels pixels...]
