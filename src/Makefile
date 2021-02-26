#-----------
#
# Targets
#
#-----------

all: main.o
	@$(CXX) $(LDFLAGS) main.o -o main.run $(LDLIBS); mv main.run $(ROOT_DIR)

main.o: main.cpp pixel.h
	@$(CXX) $(CXXFLAGS) main.cpp

clean:
	@$(RM) *.o *.run *.a *.gcda *.gcno
