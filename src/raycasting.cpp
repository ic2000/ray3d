#include "raycasting.hpp"
#include "constants.hpp"
#include "util.hpp"
#include <cmath>

namespace Ray3d {

namespace Raycasting {

sf::Vector2f find_ray_intersection(const RayDirection direction,
                                   sf::Vector2f position, const float angle,
                                   const sf::Vector2f offset,
                                   const Level &level) {
  const float tile_size{static_cast<float>(level.tile_size)};

  for (std::size_t depth_of_field{0};
       depth_of_field <
       std::max(level.tilemap.get_width(), level.tilemap.get_height());
       depth_of_field += 1) {
    sf::Vector2<std::size_t> tile_position{
        static_cast<std::size_t>(position.x / tile_size),
        static_cast<std::size_t>(position.y / tile_size)};

    if (direction == RayDirection::HORIZONTAL)
      tile_position.y -= angle > Constants::PI;
    else
      tile_position.x -=
          angle < Constants::PI * 1.5f && angle > Constants::PI_2;

    if (tile_position.x < 0 || tile_position.x >= level.tilemap.get_width() ||
        tile_position.y < 0 || tile_position.y >= level.tilemap.get_height())
      return {-1, -1};

    const auto &tile{level.tilemap.get_tile(tile_position.x, tile_position.y)};

    if (tile.has_value())
      return position;

    position.x += offset.x;
    position.y += offset.y;
  }

  return {-1, -1};
}

sf::Vector2f cast_horizontal_ray(const sf::Vector2f origin, const float angle,
                                 const Level &level) {
  const float tile_size{static_cast<float>(level.tile_size)};

  if (angle == 0 || angle == Constants::PI)
    return {-1, -1};

  float y{std::floor(origin.y / level.tile_size) * level.tile_size};
  float y_offset{-tile_size};

  if (angle < Constants::PI) {
    y += tile_size;
    y_offset = tile_size;
  }

  const float xy_ratio{-1 / std::tan(angle)};
  float x{origin.x + (xy_ratio * (origin.y - y))};
  const float x_offset{xy_ratio * -y_offset};

  return find_ray_intersection(RayDirection::HORIZONTAL, {x, y}, angle,
                               {x_offset, y_offset}, level);
}

sf::Vector2f cast_vertical_ray(const sf::Vector2f origin, const float angle,
                               const Level &level) {
  const float tile_size{static_cast<float>(level.tile_size)};

  if (angle == Constants::PI_2 || angle == Constants::PI * 1.5f)
    return {-1, -1};

  float x{std::floor(origin.x / level.tile_size) * level.tile_size};
  float x_offset{-tile_size};

  if (angle > Constants::PI * 1.5f || angle < Constants::PI_2) {
    x += tile_size;
    x_offset = tile_size;
  }

  const float xy_ratio{-std::tan(angle)};
  float y{origin.y + (xy_ratio * (origin.x - x))};
  const float y_offset{xy_ratio * -x_offset};

  return find_ray_intersection(RayDirection::VERTICAL, {x, y}, angle,
                               {x_offset, y_offset}, level);
}

std::pair<const sf::Vector2f &, float>
closest_ray_intersection(const sf::Vector2f origin, const sf::Vector2f &i1,
                         const sf::Vector2f &i2) {
  float i1_distance{-1};
  float i2_distance{-1};

  if (i1.x != -1 && i1.y != -1)
    i1_distance = Util::euclidean_distance(origin, i1);

  if (i2.x != -1 && i2.y != -1)
    i2_distance = Util::euclidean_distance(origin, i2);

  if (i1_distance == -1) {
    return {i2, i2_distance};
  } else if (i2_distance == -1) {
    return {i1, i1_distance};
  } else {
    if (i1_distance < i2_distance) {
      return {i1, i1_distance};
    } else {
      return {i2, i2_distance};
    }
  }
}

}; // namespace Raycasting

}; // namespace Ray3d
