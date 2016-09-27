#include <catch.hpp>
#include <ext/getopt.hpp>


// Test how options without argument are handled
TEST_CASE("ext::getopt - flag argumnets")
{
    int argc = 5;
    char argv_0[] = "prog";
    char argv_1[] = "-a";
    char argv_2[] = "-bv";
    char argv_3[] = "-12vv";
    char argv_4[] = "hoge";
    char* argv[] = {argv_0, argv_1, argv_2, argv_3, argv_4, nullptr};

    ext::getopt getopt;

    CHECK(getopt.optind == 1);

    int a = 0;
    int b = 0;
    int _1 = 0;
    int _2 = 0;
    int v = 0;
    int w = 0;
    int unknown = 0;

    for (int optchar; (optchar = getopt(argc, argv, "ab12vw")) != -1; )
    {
        switch (optchar)
        {
          case 'a':
            ++a;
            CHECK(getopt.optind == 2);
            CHECK(getopt.optopt == 'a');
            break;

          case 'b':
            ++b;
            CHECK(getopt.optind == 2);
            CHECK(getopt.optopt == 'b');
            break;

          case '1':
            ++_1;
            CHECK(getopt.optind == 3);
            CHECK(getopt.optopt == '1');
            break;

          case '2':
            ++_2;
            CHECK(getopt.optind == 3);
            CHECK(getopt.optopt == '2');
            break;

          case 'v':
            ++v;
            CHECK(getopt.optopt == 'v');
            break;

          case 'w':
            ++w;
            break;

          default:
            ++unknown;
            break;
        }
    }
    CHECK(getopt.optind == 4);

    CHECK(a == 1);
    CHECK(b == 1);
    CHECK(_1 == 1);
    CHECK(_2 == 1);
    CHECK(v == 3);
    CHECK(w == 0);
    CHECK(unknown == 0);
}

// Test how options with argument are handled
TEST_CASE("ext::getopt - optarg")
{
    int argc = 5;
    char argv_0[] = "prog";
    char argv_1[] = "-x";
    char argv_2[] = "hoge";
    char argv_3[] = "-ayfuga";
    char argv_4[] = "123";
    char* argv[] = {argv_0, argv_1, argv_2, argv_3, argv_4, nullptr};

    ext::getopt getopt;

    int a = 0;
    int x = 0;
    int y = 0;
    int z = 0;
    int unknown = 0;

    for (int optchar; (optchar = getopt(argc, argv, "ax:y:z:")) != -1; )
    {
        switch (optchar)
        {
          case 'a':
            ++a;
            CHECK(getopt.optind == 3);
            CHECK(getopt.optopt == 'a');
            break;

          case 'x':
            ++x;
            CHECK(getopt.optind == 3);
            CHECK(getopt.optopt == 'x');
            CHECK_THAT(getopt.optarg, Catch::Equals("hoge"));
            break;

          case 'y':
            ++y;
            CHECK(getopt.optind == 4);
            CHECK(getopt.optopt == 'y');
            CHECK_THAT(getopt.optarg, Catch::Equals("fuga"));
            break;

          case 'z':
            ++z;
            break;

          default:
            ++unknown;
            break;
        }
    }
    CHECK(getopt.optind == 4);

    CHECK(a == 1);
    CHECK(x == 1);
    CHECK(y == 1);
    CHECK(z == 0);
    CHECK(unknown == 0);
}

// Test if getopt is not messed up by single-element argv
TEST_CASE("ext::getopt - deal with single argv")
{
    int argc = 1;
    char argv_0[] = "prog";
    char* argv[] = {argv_0, nullptr};

    ext::getopt getopt;

    int a = 0;
    int x = 0;
    int unknown = 0;

    for (int optchar; (optchar = getopt(argc, argv, "ax:")) != -1; )
    {
        switch (optchar)
        {
          case 'a':
            ++a;
            break;

          case 'x':
            ++x;
            break;

          default:
            ++unknown;
            break;
        }
    }
    CHECK(getopt.optind == 1);

    CHECK(a == 0);
    CHECK(x == 0);
}

// Initial colon of optstring suppresses diagnostics by getopt
TEST_CASE("ext::getopt - initial colon of optstring")
{
    int argc = 3;
    char argv_0[] = "prog";
    char argv_1[] = "-ab";
    char argv_2[] = "-x";
    char* argv[] = {argv_0, argv_1, argv_2, nullptr};

    ext::getopt getopt;

    int a = 0;
    int x = 0;
    int question = 0;
    int colon = 0;
    int unknown = 0;

    for (int optchar; (optchar = getopt(argc, argv, ":ax:")) != -1; )
    {
        switch (optchar)
        {
          case 'a':
            ++a;
            break;

          case 'x':
            ++x;
            break;

          case '?': // -b
            ++question;
            CHECK(getopt.optind == 2);
            CHECK(getopt.optopt == 'b');
            break;

          case ':': // -x <nothing>
            ++colon;
            CHECK(getopt.optind > argc); // POSIX
            CHECK(getopt.optopt == 'x');
            break;

          default:
            ++unknown;
            break;
        }
    }
    CHECK(getopt.optind > argc); // POSIX

    CHECK(a == 1);
    CHECK(x == 0);
    CHECK(question == 1);
    CHECK(colon == 1);
    CHECK(unknown == 0);
}

// Double-hypen terminates option parsing
TEST_CASE("ext::getopt - double-hyphen")
{
    int argc = 3;
    char argv_0[] = "prog";
    char argv_1[] = "-a";
    char argv_2[] = "--";
    char argv_3[] = "-x";
    char* argv[] = {argv_0, argv_1, argv_2, argv_3, nullptr};

    ext::getopt getopt;

    int a = 0;
    int x = 0;
    int unknown = 0;

    for (int optchar; (optchar = getopt(argc, argv, "ax:")) != -1; )
    {
        switch (optchar)
        {
          case 'a':
            ++a;
            break;

          case 'x':
            ++x;
            break;

          default:
            ++unknown;
            break;
        }
    }
    CHECK(getopt.optind == 3);

    CHECK(a == 1);
    CHECK(x == 0);
    CHECK(unknown == 0);
}

// Single-hypen is considered to be a positional argument, thus terminates
// option parsing without incrementing optind (unlike double-hyphen)
TEST_CASE("ext::getopt - single-hyphen")
{
    int argc = 3;
    char argv_0[] = "prog";
    char argv_1[] = "-a";
    char argv_2[] = "-";
    char argv_3[] = "-x";
    char* argv[] = {argv_0, argv_1, argv_2, argv_3, nullptr};

    ext::getopt getopt;

    int a = 0;
    int x = 0;
    int unknown = 0;

    for (int optchar; (optchar = getopt(argc, argv, "ax:")) != -1; )
    {
        switch (optchar)
        {
          case 'a':
            ++a;
            break;

          case 'x':
            ++x;
            break;

          default:
            ++unknown;
            break;
        }
    }
    CHECK(getopt.optind == 2);

    CHECK(a == 1);
    CHECK(x == 0);
    CHECK(unknown == 0);
}
