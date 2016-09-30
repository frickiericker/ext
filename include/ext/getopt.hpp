/*
 * Re-entrant POSIX getopt(3).
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef EXT_GETOPT_HPP
#define EXT_GETOPT_HPP

#include <cstdio>
#include <cstring>

namespace ext
{
    /**
     * A function-object implementation of POSIX getopt(3) [1].
     *
     * [1]: http://pubs.opengroup.org/onlinepubs/9699919799/functions/getopt.html
     */
    struct getopt
    {
        // Associated variables.
        char* optarg = nullptr;
        int opterr = 1;
        int optind = 1;
        int optopt = 0;

        // Program name used in diagnostic messages. If this is nullptr the
        // content of argv[0] will be used.
        char const* progname = nullptr;

        /**
         * Behaves like POSIX getopt, but uses member variables instead of
         * global ones.
         */
        int operator()(int argc, char* const* argv, char const* optstring)
        {
            if (optind >= argc
                || !argv[optind]
                || argv[optind][0] != '-'
                || argv[optind][1] == '\0')
            {
                return -1;
            }

            if (std::strcmp(argv[optind], "--") == 0)
            {
                ++optind;
                return -1;
            }

            // Get next option char
            if (!curopt_ || *curopt_ == '\0')
            {
                curopt_ = argv[optind] + 1;
            }
            optopt = *curopt_++;

            // Locate option specification in optstring
            char const* option = std::strchr(optstring, optopt);

            if (!option)
            {
                // Unknown option
                if (*optstring != ':' && opterr)
                {
                    std::fprintf(stderr, "%s: unknown option -%c\n",
                                 (progname ? progname : argv[0]), optopt);
                }

                if (*curopt_ == '\0')
                {
                    ++optind;
                }

                return '?';
            }

            if (option[1] == ':')
            {
                // Handle two possibilities: -oARG or -o ARG
                optarg = (*curopt_ ? curopt_ : argv[++optind]);
                curopt_ = nullptr;
                ++optind;

                // Missing argument
                if (!optarg)
                {
                    if (*optstring != ':' && opterr)
                    {
                        std::fprintf(stderr, "%s: missing argument for -%c\n",
                                     (progname ? progname : argv[0]), optopt);
                    }
                    return (*optstring == ':') ? ':' : '?';
                }
            }
            else
            {
                if (*curopt_ == '\0')
                {
                    ++optind;
                }
            }

            return optopt;
        }

      private:

        // Points to substring of option argument in argv, starts with the
        // option character that will be processed in next call.
        char* curopt_ = nullptr;
    };
}

#endif
