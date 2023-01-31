#include "util.hpp"
#include "constants.hpp"
#include <cmath>

namespace Ray3d {

namespace Util {

float modulo(const float a, const float b) {
  if (b < 0)
    return modulo(-a, -b);

  const auto result{
      static_cast<float>(fmod(static_cast<double>(a), static_cast<double>(b)))};

  return result >= 0 ? result : result + b;
}

float to_degrees(const float radians) {
  return radians * 180.0f / Constants::PI;
}

float to_radians(const float degrees) {
  return degrees * Constants::PI / 180.0f;
}

float euclidean_distance(const sf::Vector2f &p1, const sf::Vector2f &p2) {
  const auto x_diff{p1.x - p2.x};
  const auto y_diff{p1.y - p2.y};
  return std::sqrt(x_diff * x_diff + y_diff * y_diff);
}

} // namespace Util

} // namespace Ray3d
