#pragma once

#include "level.hpp"
#include <SFML/System/Vector2.hpp>
#include <utility>

namespace Ray3d {

enum class RayDirection { HORIZONTAL, VERTICAL };

namespace Raycasting {

std::pair<sf::Vector2f, sf::Vector2i>
find_ray_intersection(const RayDirection direction, const sf::Vector2f origin,
                      const float angle, const sf::Vector2f offset,
                      const Level &level);

std::pair<sf::Vector2f, sf::Vector2i>
cast_horizontal_ray(const sf::Vector2f origin, const float angle,
                    const Level &level);

std::pair<sf::Vector2f, sf::Vector2i>
cast_vertical_ray(const sf::Vector2f origin, const float angle,
                  const Level &level);

std::pair<const sf::Vector2f &, float>
closest_ray_intersection(const sf::Vector2f origin, const sf::Vector2f &i1,
                         const sf::Vector2f &i2);

}; // namespace Raycasting

}; // namespace Ray3d
