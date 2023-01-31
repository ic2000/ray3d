#include "tilemap.hpp"
#include <cassert>

namespace Ray3d {

Tilemap::Tilemap(const std::size_t width, const std::size_t height)
    : width{width}, height{height}, tiles{width * height} {}

size_t Tilemap::get_width() const { return width; }
size_t Tilemap::get_height() const { return height; }

const std::optional<Tile> &Tilemap::get_tile(const std::size_t x,
                                             const std::size_t y) const {
  assert(x < width && y < height);
  return tiles[x + y * width];
}

void Tilemap::set_tile(const std::size_t x, const std::size_t y,
                       const Tile &&tile) {
  assert(x < width && y < height);
  tiles[x + y * width] = tile;
}

} // namespace Ray3d
