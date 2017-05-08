# ext

[![Build Status][travis-badge]][travis-url]

Header-only extensions to C++14 for my daily use.

- [Testing](#testing)
- [Modules](#modules)
- [License](#license)

[cxx-badge]: https://img.shields.io/badge/C%2B%2B-14-orange.svg
[license-badge]: http://img.shields.io/badge/license-Boost-blue.svg
[travis-badge]: https://travis-ci.org/frickiericker/ext.svg?branch=master
[travis-url]: https://travis-ci.org/frickiericker/ext

## Testing

Move to the repository root and type following commands to run tests:

    cd test
    make
    ./run

Tested compilers are:

- g++ 5.4.0
- clang++ 4.0.0 with libc++

## Modules

- Managed data storage
    - `any.hpp`: The any data type with [narrow contracts (PDF)][narrow]
    - `polymorphic_value.hpp`: Runtime polymorphism with value semantics
    - `type_map.hpp`: Quickly maps static type to a value

- Smart pointer
    - `clone_ptr.hpp`: Copying smart pointer

- Raw memory
    - `array_view.hpp`: Range view into contiguous memory region
    - `contiguous_container.hpp`: Utility functions and traits for containers
      with contiguous memory storage

- Iteration
    - `iterator_range.hpp`: Simple iterator range

- Metaprogramming
    - `type_traits.hpp`: Metafunctions

- General utilities
    - `bit_utility.hpp`: Bit operations
    - `lifetime_utility.hpp`: Utilities related to object lifetime
    - `numeric_utility.hpp`: Utility functions for numbers
    - `random_utility.hpp`: Utilities for random number generation
    - `stream_utility.hpp`: General utilities for standard streams
    - `type_conversion.hpp`: Utility functions for simple type conversions

- Pseudo-random number generation
    - `small_fast_counting_engine_v4.hpp`: Extremely fast RNG from [PractRand][pract]
    - `ziggurat_normal_distribution.hpp`: [Ziggurat algorithm][zig] for normal distribution

- Command-line utility
    - `getopt.hpp`: POSIX getopt(3) with no globals

[narrow]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4075.pdf
[pract]: http://pracrand.sourceforge.net/
[zig]: http://pracrand.sourceforge.net/

## License

Boost License 1.0
