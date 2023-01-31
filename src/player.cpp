#include "player.hpp"
#include "util.hpp"
#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace Ray3d {

Player::Player(const sf::Vector2f &pos, const Angle &angle)
    : angle{angle}, pos{pos} {
  update_delta();
}

void Player::update_delta() {
  delta = {std::cos(angle.get_radians()), std::sin(angle.get_radians())};
}

void Player::rotate(const float angle) {
  this->angle += +angle;
  update_delta();
}

void Player::set_rotation(const Angle &angle) {
  this->angle = angle;
  update_delta();
}

void Player::move_forward(const float distance) { pos += delta * distance; }
void Player::move_backward(const float distance) { pos -= delta * distance; }

void Player::move_left(const float distance) {
  pos += sf::Vector2f(delta.y, -delta.x) * distance;
}

void Player::move_right(const float distance) {
  pos += sf::Vector2f(-delta.y, delta.x) * distance;
}

const Angle &Player::get_rotation() const { return angle; }

} // namespace Ray3d
