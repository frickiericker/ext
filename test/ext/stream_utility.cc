#include <ios>
#include <sstream>

#include <catch.hpp>

#include <ext/stream_utility.hpp>


TEST_CASE("ext::save_stream_format - ")
{
    std::ostringstream stream;

    stream << 111 << '.';

    // Hexadecimal in this scope
    {
        ext::save_stream_format<char> format {stream};
        stream.flags(std::ios::hex | std::ios::right);
        stream.fill('_');
        stream.width(8);
        stream << 12345678;
    }

    stream << '.' << 222;

    CHECK(stream.str() == "111.__bc614e.222");
}
