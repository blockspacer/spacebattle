#include "authentication__ask_username.hpp"

#include "network/message.hpp"
#include "events/set_username.hpp"
#include "event.hpp"
#include "helpers/serialization.hpp"
#include "server_context.hpp"

namespace dispatchers {
namespace authentication {
namespace set_username {

bool dispatch_receive(const network::message &message, const dispatch_context &context, std::stringstream &payload) {
    events::set_username event;
    helpers::serialization::load(event, payload);

    auto session_list = context.server->get_context()->get_session_list();
    auto found = std::find_if(
            std::begin(session_list), std::end(session_list),
            [&context, &event](const std::shared_ptr<network::session> &_session) {
                return _session != context.session && _session->get_context()->get_username() == event.username;
            });
    if (found == std::end(session_list)) {
        context.session->get_context()->set_username(event.username);

        std::stringstream ss;
        helpers::serialization::save(network::message{event::set_username, true}, ss);
        context.session->deliver(network::packet::create_from_stream(ss));
        context.session->get_context()->set_state(state::in_menu);
        return true;
    } else {
        std::stringstream ss;
        helpers::serialization::save(network::message{event::set_username, false}, ss);
        helpers::serialization::save(std::string("username_already_taken"), ss);
        context.session->deliver(network::packet::create_from_stream(ss));
        return false;
    }
}

}
}
}
