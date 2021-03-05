#-----------
#
# Targets
#
#-----------

all: main.o centroid.o image.o kmeans.o videoProcess.o #$(LIB_NAME).a
	@$(CXX) $(LDFLAGS) main.o centroid.o image.o kmeans.o videoProcess.o -o main.run $(LDLIBS); mv main.run $(ROOT_DIR)

main.o: main.cpp videoProcess.h
	@$(CXX) $(CXXFLAGS) main.cpp

centroid.o: centroid.cpp centroid.h pixel.h
	@$(CXX) $(CXXFLAGS) centroid.cpp

image.o: image.cpp image.h pixel.h
	@$(CXX) $(CXXFLAGS) image.cpp

kmeans.o: kmeans.cpp pixel.h centroid.h kmeans.h
	@$(CXX) $(CXXFLAGS) kmeans.cpp

videoProcess.o: videoProcess.h pixel.h centroid.h kmeans.h image.h videoProcess.cpp
	@$(CXX) $(CXXFLAGS) videoProcess.cpp

# $(LIB_NAME).a: centroid.o image.o kmeans.o videoProcess.o
#	@$(AR) $(ARFLAGS) $(LIB_NAME).a centroid.o image.o kmeans.o videoProcess.o

clean:
	@$(RM) *.o *.run *.a *.gcda *.gcno
