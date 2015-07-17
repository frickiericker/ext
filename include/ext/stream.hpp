#ifndef EXT_STREAM_HPP
#define EXT_STREAM_HPP

#include <ios>


namespace ext
{
    template<typename Char, typename CharTraits>
    struct stream_flags_restorer
    {
        using stream_type = std::basic_ios<Char, CharTraits>;
        using flags_type = typename stream_type::fmtflags;

        stream_flags_restorer(stream_flags_restorer const&) = delete;
        stream_flags_restorer(stream_flags_restorer&&) = default;
        void operator=(stream_flags_restorer const&) = delete;

        explicit
        stream_flags_restorer(stream_type& stream)
            : stream_(stream)
            , flags_(stream.flags())
            , fill_(stream.fill())
        {
        }

        ~stream_flags_restorer()
        {
            stream_.flags(flags_);
            stream_.fill(fill_);
        }

      private:
        stream_type& stream_;
        flags_type flags_;
        Char fill_;
    };

    template<typename Char, typename CharTraits>
    auto save_stream_flags(std::basic_ios<Char, CharTraits>& ios)
    {
        return stream_flags_restorer<Char, CharTraits>(ios);
    }
}

#endif
