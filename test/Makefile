PROJECT_ROOT=   ..
GTEST_DIR=      $(PROJECT_ROOT)/dep/googletest
GTEST_OBJ=      $(GTEST_DIR)/src/gtest-all.o
GTEST_DRIVER=   $(GTEST_DIR)/src/gtest_main.o
INCLUDE_DIR=    $(PROJECT_ROOT)/include
OBJECTS=        array_view.o getopt.o iterator.o meta.o random.o smart_pointer.o stream.o

EXTRA_CXXFLAGS= -isystem $(GTEST_DIR) -isystem $(GTEST_DIR)/include \
                -I $(INCLUDE_DIR) \
                -pthread \
                -std=c++14 -W -Wall -Wextra -Wno-missing-braces -Werror -pedantic-errors \
                -O2

DRIVER=         ./unittest
RM=             rm -f

.PHONY:    run clean
.SUFFIXES: .cc .o

run: $(DRIVER)
	$(DRIVER)

clean:
	$(RM) $(DRIVER) $(OBJECTS) $(GTEST_OBJ) $(GTEST_DRIVER)

.cc.o:
	$(CXX) $(CXXFLAGS) $(EXTRA_CXXFLAGS) -c -o $@ $<

$(DRIVER): $(OBJECTS) $(GTEST_DRIVER) $(GTEST_OBJ)
	$(CXX) $(CXXFLAGS) $(EXTRA_CXXFLAGS) $(LDFLAGS) -o $(DRIVER) $(OBJECTS) $(GTEST_DRIVER) $(GTEST_OBJ)

# Dependencies
array_view.o: $(INCLUDE_DIR)/ext/array_view.hpp
getopt.o: $(INCLUDE_DIR)/ext/getopt.hpp
iterator.o: $(INCLUDE_DIR)/ext/iterator.hpp
meta.o: $(INCLUDE_DIR)/ext/meta.hpp
random.o: $(INCLUDE_DIR)/ext/random.hpp
smart_pointer.o: $(INCLUDE_DIR)/ext/smart_pointer.hpp
stream.o: $(INCLUDE_DIR)/ext/stream.hpp
