#-----------
#
# Targets
#
#-----------

all: centroid_tests.o centroid_tests.run image_tests.o image_tests.run kmeans_tests.o kmeans_tests.run
	@mv centroid_tests.run image_tests.run kmeans_tests.run $(ROOT_DIR)

centroid_tests.o: centroid_tests.cpp
	@$(CXX) $(CXXFLAGS) centroid_tests.cpp

image_tests.o: image_tests.cpp
	@$(CXX) $(CXXFLAGS) image_tests.cpp

kmeans_tests.o: kmeans_tests.cpp
	@$(CXX) $(CXXFLAGS) kmeans_tests.cpp

centroid_tests.run: centroid_tests.o
	@$(CXX) $(LDFLAGS) centroid_tests.o -o centroid_tests.run $(LDLIBS)

image_tests.run: image_tests.o
	@$(CXX) $(LDFLAGS) image_tests.o -o image_tests.run $(LDLIBS)

kmeans_tests.run: kmeans_tests.o
	@$(CXX) $(LDFLAGS) kmeans_tests.o -o kmeans_tests.run $(LDLIBS)

clean:
	@$(RM) *.o *.run *.a *.info *.gcda *.gcno *.json
