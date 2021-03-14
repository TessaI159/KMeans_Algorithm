#-----------
#
# Targets
#
#-----------

all: main.o centroid.o image.o kmeans.o videoProcess.o constants.h main.cpp color.o #$(LIB_NAME).a
	@$(CXX) $(LDFLAGS) main.o centroid.o image.o kmeans.o videoProcess.o color.o -o main.run $(LDLIBS); mv main.run $(ROOT_DIR)

main.o: main.cpp videoProcess.h
	@$(CXX) $(CXXFLAGS) main.cpp

centroid.o: centroid.cpp centroid.h pixel.h
	@$(CXX) $(CXXFLAGS) centroid.cpp

image.o: image.cpp image.h pixel.h
	@$(CXX) $(CXXFLAGS) image.cpp

kmeans.o: kmeans.cpp kmeans.h pixel.h centroid.h constants.h
	@$(CXX) $(CXXFLAGS) kmeans.cpp

videoProcess.o: videoProcess.cpp videoProcess.h pixel.h centroid.h kmeans.h image.h constants.h color.h
	@$(CXX) $(CXXFLAGS) videoProcess.cpp

color.o: color.cpp color.h constants.h
	@$(CXX) $(CXXFLAGS) color.cpp

# $(LIB_NAME).a: centroid.o image.o kmeans.o videoProcess.o
#	@$(AR) $(ARFLAGS) $(LIB_NAME).a centroid.o image.o kmeans.o videoProcess.o

clean:
	@$(RM) *.o *.run *.a *.gcda *.gcno
