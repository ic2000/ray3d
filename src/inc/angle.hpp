#pragma once
#include "constants.hpp"

namespace Ray3d {

class Angle {
  float radians{};

public:
  Angle() = default;
  explicit Angle(const float radians);
  Angle &operator=(const float radians);
  void set_degrees(const float degrees);
  Angle &operator+=(const float radians);
  Angle &operator-=(const float radians);
  float get_radians() const;
  float get_degrees() const;
};

} // namespace Ray3d
