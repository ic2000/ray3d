#pragma once

#include <SFML/System/Vector2.hpp>

namespace Ray3d {

namespace Util {

float modulo(const float a, const float b);
float to_degrees(const float radians);
float to_radians(const float degrees);
float euclidean_distance(const sf::Vector2f &p1, const sf::Vector2f &p2);

} // namespace Util

} // namespace Ray3d
