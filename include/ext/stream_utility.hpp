/*
 * General utilities for standard streams.
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef EXT_STREAM_UTILITY_HPP
#define EXT_STREAM_UTILITY_HPP

#include <ios>
#include <string>

namespace ext
{
    /**
     * Save and restore stream formatting information.
     */
    template<typename Char, typename CharTraits = std::char_traits<Char>>
    struct save_stream_format
    {
        using char_type = Char;
        using char_traits = CharTraits;
        using stream_type = std::basic_ios<Char, CharTraits>;
        using flags_type = typename stream_type::fmtflags;

        explicit
        save_stream_format(stream_type& stream)
            : stream_ {stream}
            , flags_ {stream.flags()}
            , precision_ {stream.precision()}
            , width_ {stream.width()}
            , fill_ {stream.fill()}
        {
        }

        save_stream_format(save_stream_format&&) = delete;
        save_stream_format(save_stream_format const&) = delete;
        save_stream_format& operator=(save_stream_format&&) = delete;
        save_stream_format& operator=(save_stream_format const&) = delete;

        ~save_stream_format()
        {
            stream_.flags(flags_);
            stream_.precision(precision_);
            stream_.width(width_);
            stream_.fill(fill_);
        }

      private:
        stream_type& stream_;
        flags_type flags_;
        std::streamsize precision_;
        std::streamsize width_;
        char_type fill_;
    };
}

#endif
