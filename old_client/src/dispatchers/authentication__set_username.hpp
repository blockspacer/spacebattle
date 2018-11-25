#pragma once

#include <memory>
#include "network/client.hpp"
#include "event.hpp"
#include "state.hpp"

namespace dispatchers {
namespace authentication {
namespace set_username {

bool dispatch_receive(network::client &client, std::stringstream &payload);

}
}
}