#ifndef EXT_STREAM_HPP
#define EXT_STREAM_HPP

#include <ios>


namespace ext
{
    /**
     * RAII utility for saving and restoring formatting information associated
     * to an input/output stream. The managed information is: flags, precision,
     * width, and fill. Locale, exception mask and error state are not managed.
     */
    template<typename Char, typename CharTraits>
    struct stream_flags_restorer
    {
        using char_type = Char;
        using char_traits = CharTraits;
        using stream_type = std::basic_ios<char, char_traits>;
        using flags_type = typename stream_type::fmtflags;

        stream_flags_restorer(stream_flags_restorer const&) = delete;
        stream_flags_restorer(stream_flags_restorer&&) = default;
        void operator=(stream_flags_restorer const&) = delete;

        explicit
        stream_flags_restorer(stream_type& stream)
            : stream_(stream)
            , flags_(stream.flags())
            , precision_(stream.precision())
            , width_(stream.width())
            , fill_(stream.fill())
        {
        }

        ~stream_flags_restorer()
        {
            stream_.fill(fill_);
            stream_.width(width_);
            stream_.precision(precision_);
            stream_.flags(flags_);
        }

      private:
        stream_type& stream_;
        flags_type flags_;
        std::streamsize precision_;
        std::streamsize width_;
        char_type fill_;
    };

    template<typename Char, typename CharTraits>
    auto save_stream_flags(std::basic_ios<Char, CharTraits>& ios)
    {
        return stream_flags_restorer<Char, CharTraits>(ios);
    }
}

#endif
