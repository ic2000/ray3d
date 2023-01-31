#pragma once

#include "level.hpp"
#include "view.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace Ray3d {

class TopdownView : public View {
  sf::RectangleShape player_rect{};
  sf::RectangleShape player_line{};
  const Level *level;
  sf::RenderTexture tilemap_tex{};
  sf::Sprite tilemap_sprite{};
  sf::RenderTexture rays_texture{};
  sf::Sprite rays_sprite{};
  sf::View view{};

  void draw_tilemap();

public:
  TopdownView(const sf::Vector2f &size, const Level &level);
  void update_size(const sf::Vector2f &size) override;
  void update_player_position();
  void update_player_rotation();
  void clear_rays();

  void cast_ray(const sf::Vector2f &origin, const float angle,
                const float distance);

  void draw(sf::RenderTarget *target) override;
  const sf::View &get_view() const override;
};

} // namespace Ray3d
