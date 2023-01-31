#pragma once

#include "player.hpp"
#include "rapidjson/document.h"
#include "tilemap.hpp"
#include <string>
#include <unordered_map>

namespace Ray3d {

struct Level {
  int tile_size{};
  Tilemap tilemap{};
  Player player{};
  explicit Level(const rapidjson::Document &doc);
};

} // namespace Ray3d
