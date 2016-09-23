#include <ios>
#include <sstream>

#include <catch.hpp>

#include <ext/stream.hpp>


TEST_CASE("ext::save_stream_flags / flag recovery on scope out")
{
    std::stringstream stream;

    // Some stream flags are changed within this block.
    {
        auto stream_flags = ext::save_stream_flags(stream);
        stream.flags(std::ios::hex | std::ios::right);
        stream.fill('0');
        stream.width(4);
        stream << 255;
        CHECK(stream.str() == "00ff");
    }

    // Now the default stream flags should be recovered.
    stream << 123;
    CHECK(stream.str() == "00ff123");
}
