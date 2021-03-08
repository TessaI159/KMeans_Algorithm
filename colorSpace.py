import os
import sys
import math

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


def simpleEuclideanRGBDistance(color1, color2):
    return math.sqrt(((color1.rgb.sR - color2.rgb.sR)**2) + ((color1.rgb.sG - color2.rgb.sG)**2) + ((color1.rgb.sB - color2.rgb.sB)**2))


def weightedEuclideanRGBDistance(color1, color2):
    if(((color1.rgb.sR + color2.rgb.sR) / 2) < 128):
        r = 2.0
        b = 3.0
    else:
        r = 3.0
        b = 2.0
    return math.sqrt(((r * (color1.rgb.sR - color2.rgb.sR)**2) + (4.0 * (color1.rgb.sG - color2.rgb.sR)**2) + (b * (color1.rgb.sB - color2.rgb.sB)**2)))


def redmeanEuclideanRGBDistance(color1, color2):
    rAverage = ((color1.rgb.sR + color2.rgb.sR) / 2)

    return math.sqrt(((2 + (rAverage / 256) * (color1.rgb.sR - color2.rgb.sR)**2) + ((4 * (color1.rgb.sG - color2.rgb.sG)**2)) + ((2 + ((255 - rAverage) / 256)) * (color1.rgb.sB - color2.rgb.sB)**2)))


def taxicabLABDistance(color1, color2):
    return math.sqrt(((color1.lab.a - color2.lab.a)**2) + ((color1.lab.b - color2.lab.b)**2)) + abs(color1.lab.l - color2.lab.l)


def deltaCDistance(color1, color2):
    return math.sqrt((color2.lab.a**2) + (color2.lab.b**2)) - math.sqrt((color1.lab.a**2) + (color1.lab.b**2))


def deltaHDistance(color1, color2):
    return math.sqrt(((color2.lab.a - color2.lab.b)**2) + ((color1.lab.b - color1.lab.a)**2) - ((deltaCDistance(color1, color2))**2))


def deltaECIEDistance(color1, color2):
    return math.sqrt(((color1.lab.l - color2.lab.l)**2) + ((color1.lab.l - color2.lab.a)**2) + ((color1.lab.b - color2.lab.b)**2))


def deltaE94Distance(color1, color2):
    whtL = 1
    whtC = 1
    whtH = 1
    xC1 = math.sqrt((color1.lab.a**2) + (color1.lab.b**2))
    xC2 = math.sqrt((color2.lab.a**2) + (color2.lab.b**2))
    xDL = color2.lab.l - color1.lab.l
    xDC = xC2 - xC1
    xDE = deltaECIEDistance(color1, color2)
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

def LabtoHue(a, b):
    if(a >= 0 and b == 0):
        return 0
    elif(a < 0 and b == 0):
        return 180
    elif(a == 0 and b > 0):
        return 90
    elif(a == 0 and b < 0):
        return 270
    elif(a > 0 and b > 0):
        bias = 0
    elif(a < 0):
        bias = 180
    elif(a > 0 and b < 0):
        bias = 360

    return (math.degrees(math.atan(b / a)) + bias)


def deltaE00Distance(color1, color2):
    whtL = 1
    whtC = 1
    whtH = 1

    xC1 = math.sqrt((color1.lab.a**2) + (color1.lab.b**2))
    xC2 = math.sqrt((color2.lab.a**2) + (color2.lab.b**2))
    xCX = (xC1 + xC2) / 2
    xGX = 0.5 * (1 - math.sqrt((xCX**7) / ((xCX**7) + (25**7))))
    xNN = (1 + xGX) * color1.lab.a
    xC1 = math.sqrt((xNN**2) + (color1.lab.b**2))
    xH1 = LabtoHue(xNN, color1.lab.b)
    xNN = (1 + xGX) * color2.lab.a
    xC2 = math.sqrt((xNN**2) + (color2.lab.b**2))
    xH2 = LabtoHue(xNN, color2.lab.b)
    xDL = color2.lab.l - color1.lab.l
    xDC = xC2 - xC1

    if((xC1 * xC2) == 0):
        xDH = 0
    else:
        xNN = round(xH2 - xH1, 12)
        if((abs(xNN)) <= 180):
            xDH = xH2 - xH1
        else:
            if(xNN > 180):
                xDH = xH2 - xH1 - 360
            else:
                xDH = xH2 - xH1 + 360

    xDH = 2 * math.sqrt(xC1 * xC2) * math.sin(math.radians(xDH / 2))
    xLX = (color1.lab.l + color2.lab.l) / 2
    xCY = (xC1 + xC2) / 2
    if((xC1 * xC2) == 0):
        xHX = xH1 + xH2
    else:
        xNN = abs(round((xH1 - xH2), 12))
        if(xNN > 180):
            if((xH2 + xH1) < 360):
                xHX = xH1 + xH2 + 360
            else:
                xHX = xH1 + xH2 - 360
        else:
            xHX = xH1 + xH2
        xHx /= 2

    # This line isn't even close to done
    xTX = 1 - 0.17 * math.cos(math.radians(xHX - 30)) + 0.24 * math.cos()

def deltaCMCDistance(color1, color2):
    pass


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
            centroid.r = 0
        if(centroid.g < 0):
            centroid.g = 0
        if(centroid.b < 0):
            centroid.b = 0

    return (centroidList, targetPixels, executionTime)



  

def compareOutputs(centroidListSmall, centroidListLarge, executionSmall, executionLarge, pixelsSmall, pixelsLarge):
    for x in range(len((centroidListSmall))):
        pass

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

color1 = Color(255, 193, 204)
color2 = Color(225, 221, 172)


# print(f'Simple RGB Euclidean: {simpleEuclideanRGBDistance(color1, color2)}')
# print(f'Weighted RGB Euclidean: {weightedEuclideanRGBDistance(color1, color2)}')
# print(f'Readmean RGB Euclidean: {redmeanEuclideanRGBDistance(color1, color2)}')
# print (f'Taxicab L*ab: {taxicabLABDistance(color1, color2)}')
# print(f'Delta E \'76 L*ab: {deltaE76LABDistance(color1, color2)}')


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
#     os.system(f'./main.run {pixel} {defaultMinPixels} {videoName}')
#     pixelStr = str(pixel)
#     os.system(f'mv output output{pixelStr}_{videoName[:-4]}')
#     centroidListOutput = createCentroidList(f'output{pixelStr}_{videoName[:-4]}')
    
#     centroidListList.append(centroidListOutput[0])
#     centroidListExecution.append(centroidListOutput[2])

# for x in range(len(centroidListList)):
#     if(x != largestPixelIndex):
#         compareOutputs(centroidListList[x], centroidListList[largestPixelIndex], centroidListExecution[x], centroidListExecution[largestPixelIndex], pixelList[x], pixelList[largestPixelIndex] )

# script video [pixels pixels pixels...]
