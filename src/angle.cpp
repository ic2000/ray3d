#include "angle.hpp"
#include "util.hpp"

namespace Ray3d {

Angle::Angle(const float radians)
    : radians{Util::modulo(radians, Constants::TAU)} {}

Angle &Angle::operator=(const float radians) {
  this->radians = Util::modulo(radians, Constants::TAU);
  return *this;
}

void Angle::set_degrees(const float degrees) {
  radians = Util::to_radians(degrees);
}

Angle &Angle::operator+=(const float radians) {
  this->radians = Util::modulo(this->radians += radians, Constants::TAU);
  return *this;
}

Angle &Angle::operator-=(const float radians) {
  this->radians = Util::modulo(this->radians -= radians, Constants::TAU);
  return *this;
}

float Angle::get_radians() const { return radians; }
float Angle::get_degrees() const { return Util::to_degrees(radians); }

} // namespace Ray3d
