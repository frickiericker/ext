/*
 * Command-option parsing.
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef EXT_GETOPT_HPP
#define EXT_GETOPT_HPP

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
        /**
         * Argument of current option.
         */
        char* optarg = nullptr;

        /**
         * Index of the next element of argv to be processed.
         */
        int optind = 1;

        /**
         * Currently processed option character.
         */
        int optopt = 0;

        /**
         * Behaves like POSIX getopt, using member variables instead of globals.
         *
         * POSIX requires that getopt should write diagnostic message to stderr
         * on error, but this implementation does not do that. This behaviour is
         * equivalent to a conforming implementation with `opterr` set to zero.
         * Error is reported by returning ':' or '?' and setting `optopt` as
         * specified by the standard.
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
