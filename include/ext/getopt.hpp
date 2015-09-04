#ifndef EXT_GETOPT_HPP
#define EXT_GETOPT_HPP

#include <cassert>
#include <cstdio>
#include <cstring>


namespace ext
{
    /*
     * A function-object implementation of POSIX getopt(1).
     *
     * Reference:
     *  http://pubs.opengroup.org/onlinepubs/9699919799/functions/getopt.html
     */
    struct getopt
    {
        // Associated variables.
        char* optarg = nullptr;
        int opterr = 1;
        int optind = 0;
        int optopt = 0;

        // Program name used in diagnostic messages. If this is nullptr the
        // content of argv[0] will be used.
        char const* progname = nullptr;

        /*
         * Behaves like POSIX getopt(1), but uses the member variables instead
         * of global ones.
         */
        int operator()(int argc, char* const* argv, char const* optstring)
        {
            if (optind >= argc)
            {
                return -1;
            }

            // Parse next option out of argv if necessary
            if (!curopt || *curopt == '\0')
            {
                if (get_next_option(argc, argv) == -1)
                {
                    return -1; // no more option arguments
                }
            }
            optopt = *curopt++;

            // Locate option specification in optstring
            if (char const* option = std::strchr(optstring, optopt))
            {
                if (option[1] == ':')
                {
                    // Handle two possibilities: -oARG or -o ARG
                    optarg = (*curopt ? curopt : argv[++optind]);
                    curopt = nullptr;

                    // Missing argument
                    if (!optarg)
                    {
                        if (optstring[0] != ':' && opterr)
                        {
                            std::fprintf(stderr, "%s: missing argument for -%c\n",
                                         (progname ? progname : argv[0]), optopt);
                        }
                        return (optstring[0] == ':') ? ':' : '?';
                    }
                }
            }
            else
            {
                // Unknown option
                if (optstring[0] != ':' && opterr)
                {
                    std::fprintf(stderr, "%s: unknown option -%c\n",
                                 (progname ? progname : argv[0]), optopt);
                }
                return '?';
            }

            return optopt;
        }

      private:

        // Points to substring of option argument in argv, starts with the
        // option character that is currently processed.
        char* curopt = nullptr;

        /*
         * Locate next option argument in argv and sets optind and curopt
         * accordingly.
         */
        int get_next_option(int argc, char* const* argv)
        {
            assert(optind < argc); // note: argv[argc] is valid

            // Extract next option out of argv, if any
            ++optind;

            if (!argv[optind]               // end of arguments
                || argv[optind][0] != '-'   // non-option
                || argv[optind][1] == '\0') // single '-' (=> non-option)
            {
                return -1;
            }

            // Double hyphen designates the end of options
            if (std::strcmp(argv[optind], "--") == 0)
            {
                ++optind;
                return -1;
            }

            // Set curopt point to option char
            assert(argv[optind][0] == '-');
            curopt = argv[optind] + 1;
            assert(curopt && *curopt != '\0');
            return *curopt;
        }
    };
}

#endif
