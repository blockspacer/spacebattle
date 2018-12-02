#pragma once

#include <memory>
#include "state.hpp"
#include "event.hpp"
#include "message.hpp"

namespace godot {
class client;
}

namespace dispatcher {

bool dispatch(cereal::BinaryInputArchive &archive,
              const message &message,
              godot::client &client);

}
