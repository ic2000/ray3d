#pragma once

#include "raycasting.hpp"
#include "view.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace Ray3d {

class GameView : public View {
  static constexpr unsigned int CROSSHAIR_SIZE{16};
  static constexpr float FOG_DISTANCE{100};

  sf::RenderTexture hud_tex{};
  sf::Sprite hud_sprite{};
  sf::RenderTexture crosshair_tex{};
  sf::Sprite crosshair_sprite{};
  sf::RectangleShape sky{};
  sf::RectangleShape floor{};
  sf::RenderTexture render_tex{};
  sf::Sprite render_sprite{};
  sf::View view{};

  void draw_crosshair();

public:
  explicit GameView(const sf::Vector2f &size);
  void update_size(const sf::Vector2f &size) override;

  void clear_rays();

  float cast_ray(const int index, const float origin_angle,
                const float ray_angle, float distance, const int fov,
                const int tile_size, RayDirection ray_direction,
                const Tile &tile, float current_height = -1);

  void draw(sf::RenderTarget *target) override;
  const sf::View &get_view() const override;
};

} // namespace Ray3d
