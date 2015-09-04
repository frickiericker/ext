#include <gtest/gtest.h>

#include <ext/getopt.hpp>


// Test how options without argument are handled
TEST(GetOpt, FlagArguments)
{
    int argc = 5;
    char argv_0[] = "prog";
    char argv_1[] = "-a";
    char argv_2[] = "-bv";
    char argv_3[] = "-12vv";
    char argv_4[] = "hoge";
    char* argv[] = {argv_0, argv_1, argv_2, argv_3, argv_4, nullptr};

    ext::getopt getopt;

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
            ASSERT_EQ(1, getopt.optind);
            ASSERT_EQ('a', getopt.optopt);
            break;

          case 'b':
            ++b;
            ASSERT_EQ(2, getopt.optind);
            ASSERT_EQ('b', getopt.optopt);
            break;

          case '1':
            ++_1;
            ASSERT_EQ(3, getopt.optind);
            ASSERT_EQ('1', getopt.optopt);
            break;

          case '2':
            ++_2;
            ASSERT_EQ(3, getopt.optind);
            ASSERT_EQ('2', getopt.optopt);
            break;

          case 'v':
            ++v;
            ASSERT_EQ('v', getopt.optopt);
            break;

          case 'w':
            ++w;
            break;

          default:
            ++unknown;
            break;
        }
    }
    ASSERT_EQ(4, getopt.optind);

    ASSERT_EQ(1, a);
    ASSERT_EQ(1, b);
    ASSERT_EQ(1, _1);
    ASSERT_EQ(1, _2);
    ASSERT_EQ(3, v);
    ASSERT_EQ(0, w);
    ASSERT_EQ(0, unknown);
}

// Test how options with argument are handled
TEST(GetOpt, Optarg)
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
            ASSERT_EQ(3, getopt.optind);
            ASSERT_EQ('a', getopt.optopt);
            break;

          case 'x':
            ++x;
            ASSERT_EQ(1, getopt.optind);
            ASSERT_EQ('x', getopt.optopt);
            ASSERT_STREQ("hoge", getopt.optarg);
            break;

          case 'y':
            ++y;
            ASSERT_EQ(3, getopt.optind);
            ASSERT_EQ('y', getopt.optopt);
            ASSERT_STREQ("fuga", getopt.optarg);
            break;

          case 'z':
            ++z;
            break;

          default:
            ++unknown;
            break;
        }
    }
    ASSERT_EQ(4, getopt.optind);

    ASSERT_EQ(1, a);
    ASSERT_EQ(1, x);
    ASSERT_EQ(1, y);
    ASSERT_EQ(0, z);
    ASSERT_EQ(0, unknown);
}

// Test if getopt is not messed up by single-element argv
TEST(GetOpt, SingleArgv)
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
    ASSERT_EQ(1, getopt.optind);

    ASSERT_EQ(0, a);
    ASSERT_EQ(0, x);
}

// Initial colon of optstring suppresses diagnostics by getopt
TEST(GetOpt, InitialColon)
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

          case '?':
            ++question;
            ASSERT_EQ(1, getopt.optind);
            ASSERT_EQ('b', getopt.optopt);
            break;

          case ':':
            ++colon;
            ASSERT_EQ(2, getopt.optind);
            ASSERT_EQ('x', getopt.optopt);
            break;

          default:
            ++unknown;
            break;
        }
    }
    ASSERT_EQ(3, getopt.optind);

    ASSERT_EQ(1, a);
    ASSERT_EQ(0, x);
    ASSERT_EQ(1, question);
    ASSERT_EQ(1, colon);
    ASSERT_EQ(0, unknown);
}
