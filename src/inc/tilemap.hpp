#pragma once

#include <optional>
#include <vector>

namespace Ray3d {

class Tile {};

class Tilemap {
  size_t width{};
  size_t height{};
  std::vector<std::optional<Tile>> tiles{};

public:
  Tilemap() = default;
  Tilemap(const std::size_t width, const std::size_t height);
  size_t get_width() const;
  size_t get_height() const;

  const std::optional<Tile> &get_tile(const std::size_t x,
                                      const std::size_t y) const;

  void set_tile(const std::size_t x, const std::size_t y, const Tile &&tile);
};

} // namespace Ray3d
