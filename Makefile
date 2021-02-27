#-----------
#
# Variables
#
#-----------

CXX=g++
CXXFLAGS=-g -Wall -std=c++14 -c --coverage -I/usr/local/include/opencv4/
LDFLAGS=-g --coverage
LDLIBS=-L/usr/local/lib/OpenCV -L/usr/lib/x86_64-linux-gnu -L$(SRC_DIR) \
-l:libboost_unit_test_framework.a -l:libopencv_core.so -l:libopencv_imgproc.so \
-l:libopencv_imgcodecs.so -l:libopencv_video.so -l:libopencv_videoio.so -l:$(LIB_NAME).a


LIB_NAME=kmeans
AR=ar
ARFLAGS=-cr
ROOT_DIR=/home/tess/Code/KMeans
SRC_DIR=$(ROOT_DIR)/src
TEST_DIR=$(ROOT_DIR)/t


#-----------
#
# Exports
#
#-----------


export CXX
export CXXFLAGS
export LDFLAGS
export LDLIBS
export LIB_NAME
export ROOT_DIR
export SRC_DIR
export TEST_DIR


#-----------
#
# Targets
#
#-----------

all: coverage_clean main test

no_tests: coverage_clean main

main:
	@$(MAKE) -C $(SRC_DIR)

test: main
	@$(MAKE) -C $(TEST_DIR)

clean: main_clean test_clean
	@$(RM) *.o *.run *.a *.info *.gcda *.gcno *.json; lcov --no-external -d $(PWD) -z -q

main_clean:
	@$(MAKE) -C $(SRC_DIR) clean

test_clean:
	@$(MAKE) -C $(TEST_DIR) clean

coverage_clean:
	@$(RM) -rf *.gcno *.gcda src/*.gcda src/*.gcno t/*.gcda t/*.gcno coverage_info

check: all
	@./centroid_tests.run; ./image_tests.run

coverage: clean check
	@lcov -c -d $(PWD) -o coverage.info -q --no-external && genhtml -o coverage_info coverage.info && firefox ./coverage_info/index.html
