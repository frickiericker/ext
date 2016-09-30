PROJECT_ROOT = ..
INCLUDE_DIR = $(PROJECT_ROOT)/include

TARGET = ./run

OBJECTS = \
    main.o \
    ext/any.o \
    ext/array_view.o \
    ext/bit_utility.o \
    ext/clone_ptr.o \
    ext/contiguous_container.o \
    ext/getopt.o \
    ext/iterator_range.o \
    ext/lifetime_utility.o \
    ext/numeric_utility.o \
    ext/polymorphic_value.o \
    ext/random_utility.o \
    ext/small_fast_counting_engine_v4.o \
    ext/stream_utility.o \
    ext/type_conversion.o \
    ext/type_map.o \
    ext/type_traits.o \
    ext/ziggurat_normal_distribution.o

CXXFLAGS = \
    -std=c++14 \
    -pedantic-errors \
    -Wall \
    -Wextra \
    -Werror \
    -fsanitize=address,undefined \
    -g \
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
ext/any.o: \
    $(INCLUDE_DIR)/ext/any.hpp \
    $(INCLUDE_DIR)/ext/clone_ptr.hpp

ext/array_view.o: \
    $(INCLUDE_DIR)/ext/array_view.hpp \
    $(INCLUDE_DIR)/ext/contiguous_container.hpp \
    $(INCLUDE_DIR)/ext/iterator_range.hpp \
    $(INCLUDE_DIR)/ext/type_traits.hpp

ext/bit_utility.o: \
    $(INCLUDE_DIR)/ext/bit_utility.hpp

ext/clone_ptr.o: \
    $(INCLUDE_DIR)/ext/clone_ptr.hpp

ext/contiguous_container.o: \
    $(INCLUDE_DIR)/ext/contiguous_container.hpp

ext/getopt.o: \
    $(INCLUDE_DIR)/ext/getopt.hpp

ext/iterator_range.o: \
    $(INCLUDE_DIR)/ext/iterator_range.hpp

ext/lifetime_utility.o: \
    $(INCLUDE_DIR)/ext/lifetime_utility.hpp

ext/numeric_utility.o: \
    $(INCLUDE_DIR)/ext/numeric_utility.hpp

ext/polymorphic_value.o: \
    $(INCLUDE_DIR)/ext/polymorphic_value.hpp \
    $(INCLUDE_DIR)/ext/lifetime_utility.hpp

ext/random_utility.o: \
    $(INCLUDE_DIR)/ext/random_utility.hpp

ext/small_fast_counting_engine_v4.o: \
    $(INCLUDE_DIR)/ext/small_fast_counting_engine_v4.hpp \
    $(INCLUDE_DIR)/ext/bit_utility.hpp \
    $(INCLUDE_DIR)/ext/random_utility.hpp \
    $(INCLUDE_DIR)/ext/stream_utility.hpp

ext/stream_utility.o: \
    $(INCLUDE_DIR)/ext/stream_utility.hpp

ext/type_conversion.o: \
    $(INCLUDE_DIR)/ext/type_conversion.hpp

ext/type_map.o: \
    $(INCLUDE_DIR)/ext/type_map.hpp

ext/type_traits.o: \
    $(INCLUDE_DIR)/ext/type_traits.hpp

ext/ziggurat_normal_distribution.o: \
    $(INCLUDE_DIR)/ext/ziggurat_normal_distribution.hpp
