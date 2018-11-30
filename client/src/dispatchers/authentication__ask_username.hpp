#pragma once

#include <memory>
#include "client.hpp"
#include "event.hpp"
#include "state.hpp"

namespace dispatchers {
namespace authentication {
namespace ask_username {

bool dispatch(godot::client &client, std::stringstream &payload);

}
}
}