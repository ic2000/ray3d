#pragma once

#include "view.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>

namespace Ray3d {

class DiagnosticsView : public View {
  sf::Text fps_text{};
  sf::View view{};

public:
  explicit DiagnosticsView(const sf::Vector2f &size, const sf::Font &font);
  void update_size(const sf::Vector2f &size) override;
  void draw(sf::RenderTarget *target) override;
  void update_fps(const float fps);
  const sf::View &get_view() const override;
};

} // namespace Ray3d
