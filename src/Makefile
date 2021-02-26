#-----------
#
# Targets
#
#-----------

all: main.o centroid.o $(LIB_NAME).a
	@$(CXX) $(LDFLAGS) main.o -o main.run $(LDLIBS); mv main.run $(ROOT_DIR)

main.o: main.cpp pixel.h centroid.o
	@$(CXX) $(CXXFLAGS) main.cpp

centroid.o:
	@$(CXX) $(CXXFLAGS) centroid.cpp

$(LIB_NAME).a: centroid.o
	@$(AR) $(ARFLAGS) $(LIB_NAME).a centroid.o

clean:
	@$(RM) *.o *.run *.a *.gcda *.gcno
