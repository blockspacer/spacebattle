#include <iostream>
#include <sstream>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <cereal/archives/binary.hpp>

#include "network/server.hpp"

int main() {
    try {
        asio::io_context io_context;
        asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), 42500);
        auto server = std::make_shared<network::server>(io_context, endpoint);
        io_context.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}