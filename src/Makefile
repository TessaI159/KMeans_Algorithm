#-----------
#
# Targets
#
#-----------

all: main.o centroid.o image.o kmeans.o videoProcess.o color.o constants.h main.cpp \
	difference.o Lch.o Lab.o sRGB.o XYZ.o #$(LIB_NAME).a
	@$(CXX) $(LDFLAGS) main.o centroid.o image.o kmeans.o videoProcess.o color.o \
	Lch.o Lab.o XYZ.o sRGB.o difference.o -o main.run $(LDLIBS); mv main.run $(ROOT_DIR)

main.o: main.cpp videoProcess.h
	@$(CXX) $(CXXFLAGS) main.cpp

centroid.o: centroid.cpp centroid.h pixel.h
	@$(CXX) $(CXXFLAGS) centroid.cpp

image.o: image.cpp image.h pixel.h
	@$(CXX) $(CXXFLAGS) image.cpp

kmeans.o: kmeans.cpp kmeans.h pixel.h centroid.h constants.h
	@$(CXX) $(CXXFLAGS) kmeans.cpp

videoProcess.o: videoProcess.cpp videoProcess.h pixel.h centroid.h kmeans.h image.h \
	constants.h color.h difference.h
	@$(CXX) $(CXXFLAGS) videoProcess.cpp

color.o: color.cpp color.h constants.h Lch.h XYZ.h Lab.h sRGB.h difference.h
	@$(CXX) $(CXXFLAGS) color.cpp

Lch.o: Lch.cpp Lch.h
	@$(CXX) $(CXXFLAGS) Lch.cpp

Lab.o: Lab.cpp Lab.h
	@$(CXX) $(CXXFLAGS) Lab.cpp

sRGB.o: sRGB.cpp sRGB.h
	@$(CXX) $(CXXFLAGS) sRGB.cpp

XYZ.o: XYZ.cpp XYZ.h
	@$(CXX) $(CXXFLAGS) XYZ.cpp

difference.o: difference.cpp difference.h
	@$(CXX) $(CXXFLAGS) difference.cpp

# If this is ever uncommented, it needs a major update
# $(LIB_NAME).a: centroid.o image.o kmeans.o videoProcess.o
#	@$(AR) $(ARFLAGS) $(LIB_NAME).a centroid.o image.o kmeans.o videoProcess.o

clean:
	@$(RM) *.o *.run *.a *.gcda *.gcno
