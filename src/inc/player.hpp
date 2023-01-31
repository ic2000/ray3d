#pragma once

#include "angle.hpp"
#include <SFML/System/Vector2.hpp>

namespace Ray3d {

class Player {
  Angle angle{};
  sf::Vector2f delta{};

public:
  static constexpr float SIZE{7.5f};
  sf::Vector2f pos{};

  Player() = default;
  Player(const sf::Vector2f &pos, const Angle &angle);
  void update_delta();
  void rotate(const float angle);
  void set_rotation(const Angle &angle);
  void move_forward(const float distance);
  void move_backward(const float distance);
  void move_left(const float distance);
  void move_right(const float distance);
  const Angle &get_rotation() const;
};

} // namespace Ray3d
