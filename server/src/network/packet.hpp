#pragma once

#include <cstddef>

namespace network {
    class packet {
    public:
        enum { header_length = 4 };
        enum { max_body_length = 512 };

        packet();

        const char *data() const;

        char *data();

        std::size_t length() const;

        const char *body() const;

        char *body();

        std::size_t body_length() const;

        void body_length(std::size_t new_length);

        bool decode_header();

        void encode_header();

    private:
        char data_[header_length + max_body_length];
        std::size_t body_length_;
    };
}
