#pragma once

#include <SFML/Graphics/Color.hpp>

namespace Ray3d {

struct Tile {
  float height{};
  sf::Color floor_color{};
  sf::Color wall_color{};
};

} // namespace Ray3d
