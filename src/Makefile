#-----------
#
# Targets
#
#-----------

all: main.o centroid.o image.o $(LIB_NAME).a
	@$(CXX) $(LDFLAGS) main.o centroid.o image.o -o main.run $(LDLIBS); mv main.run $(ROOT_DIR)

main.o: main.cpp pixel.h centroid.o image.o
	@$(CXX) $(CXXFLAGS) main.cpp

centroid.o: centroid.cpp centroid.h pixel.h
	@$(CXX) $(CXXFLAGS) centroid.cpp

image.o: image.cpp image.h pixel.h
	@$(CXX) $(CXXFLAGS) image.cpp

$(LIB_NAME).a: centroid.o image.o
	@$(AR) $(ARFLAGS) $(LIB_NAME).a centroid.o image.o

clean:
	@$(RM) *.o *.run *.a *.gcda *.gcno
