#include "dispatcher.hpp"

#include <map>
#include <functional>
#include <stdexcept>
#include "helpers/serialization.hpp"
#include "network/client.hpp"
#include "network/message.hpp"
#include "dispatchers/authentication__ask_username.hpp"
#include "dispatchers/authentication__set_username.hpp"

namespace dispatcher {

using dispatcher_send_function_type = bool(network::client &);

using dispatcher_receive_function_type = bool(network::client &, std::stringstream &);

struct dispatcher_struct {
    std::function<dispatcher_send_function_type> dispatch_send;
    std::function<dispatcher_receive_function_type> dispatch_receive;
};

static const std::map<state, std::map<event, dispatcher_struct>> _dispatchers = { // NOLINT(cert-err58-cpp)
        {state::authentication,
                {
                        {event::ask_username,
                                {
                                        nullptr,
                                        dispatchers::authentication::ask_username::dispatch_receive
                                }
                        },
                        {event::set_username,
                                {
                                        nullptr,
                                        dispatchers::authentication::set_username::dispatch_receive
                                }
                        }
                }
        }
};

bool find_dispatcher_and_run(const state &state,
                             const event &event,
                             const std::function<bool(const dispatcher_struct &)> &f) {
    try {
        return f(_dispatchers.at(state).at(event));
    } catch (std::out_of_range &) {
        return false;
    }
}

bool dispatch_send(const event &event,
                   network::client &client) {

    std::cout << "Event sent: " << event << ", dispatching status: ";

    auto b = find_dispatcher_and_run(
            client.get_context().get_state(), event,
            [&](const dispatcher_struct &e) {
                if (e.dispatch_send != nullptr) return e.dispatch_send(client);
                else return false;
            });
    std::cout << b << "." << std::endl;
    return b;
}

bool dispatch_receive(const network::packet &packet,
                      network::client &client) {
    network::message message;
    {
        std::stringstream ss(std::string(packet.get_body(), MESSAGE_LENGTH));
        helpers::serialization::load(message, ss);
    }

    std::cout << "Event received: " << message.type << ", dispatching status: ";

    std::stringstream payload;

    if (packet.get_body_length() - MESSAGE_LENGTH > 0) {
        payload.write(packet.get_body() + MESSAGE_LENGTH,
                      packet.get_body_length() - MESSAGE_LENGTH);
    }

    auto b = find_dispatcher_and_run(
            client.get_context().get_state(), message.type,
            [&](const dispatcher_struct &e) {
                if (e.dispatch_receive != nullptr) return e.dispatch_receive(client, payload);
                else return false;
            });
    std::cout << b << "." << std::endl;
    return b;
}

}