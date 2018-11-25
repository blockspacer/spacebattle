#pragma once

#include <deque>
#include <asio.hpp>
#include "packet.hpp"
#include "context.hpp"

namespace network {

class client
        : public std::enable_shared_from_this<client> {
public:
    client(asio::io_service &io_service,
           asio::ip::tcp::resolver::iterator endpoint_iterator);

    void deliver(const network::packet &msg);

    void close();

    context &get_context();

private:
    void do_connect(asio::ip::tcp::resolver::iterator endpoint_iterator);

    void do_read();

    void do_write();

    asio::io_service &io_service_;
    asio::ip::tcp::socket socket_;
    network::packet read_packet_;
    std::deque<network::packet> write_packets_;
    context context_;
};

}