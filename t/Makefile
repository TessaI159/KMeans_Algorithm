#-----------
#
# Targets
#
#-----------

all: centroid_tests.o centroid_tests.run
	@mv centroid_tests.run $(ROOT_DIR)

centroid_tests.o: centroid_tests.cpp
	@$(CXX) $(CXXFLAGS) centroid_tests.cpp

centroid_tests.run: centroid_tests.o
	@$(CXX) $(LDFLAGS) centroid_tests.o -o centroid_tests.run $(LDLIBS)

clean:
	@$(RM) *.o *.run *.a *.info *.gcda *.gcno *.json
