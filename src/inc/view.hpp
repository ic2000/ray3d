#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

namespace Ray3d {

class View {
public:
  virtual void draw(sf::RenderTarget *target) = 0;
  virtual void update_size(const sf::Vector2f &size) = 0;
  virtual const sf::View &get_view() const = 0;
  virtual ~View() = default;
};

} // namespace Ray3d
