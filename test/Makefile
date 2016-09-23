PROJECT_ROOT = ..
INCLUDE_DIR = $(PROJECT_ROOT)/include

TARGET = ./run

OBJECTS = \
    main.o \
    ext/array_view.o \
    ext/getopt.o \
    ext/iterator.o \
    ext/meta.o \
    ext/random.o \
    ext/smart_pointer.o \
    ext/stream.o

CXXFLAGS = \
    -std=c++14 \
    -pedantic-errors \
    -Wall \
    -Wextra \
    -Werror \
    -fsanitize=address,undefined \
    -O2 \
    $(EXTRA_CXXFLAGS) \
    -I $(INCLUDE_DIR)

.PHONY: all clean

all: $(TARGET)
	@:

clean:
	rm -f $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# Dependencies
ext/array_view.o: $(INCLUDE_DIR)/ext/array_view.hpp
ext/getopt.o: $(INCLUDE_DIR)/ext/getopt.hpp
ext/iterator.o: $(INCLUDE_DIR)/ext/iterator.hpp
ext/meta.o: $(INCLUDE_DIR)/ext/meta.hpp
ext/random.o: $(INCLUDE_DIR)/ext/random.hpp
ext/smart_pointer.o: $(INCLUDE_DIR)/ext/smart_pointer.hpp
ext/stream.o: $(INCLUDE_DIR)/ext/stream.hpp
