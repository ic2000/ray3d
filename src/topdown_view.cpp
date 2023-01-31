#include "topdown_view.hpp"
#include "player.hpp"
#include "util.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>

namespace Ray3d {

void TopdownView::draw_tilemap() {
  tilemap_tex.clear(sf::Color::Transparent);
  const float tile_size_f{static_cast<float>(this->level->tile_size)};
  const auto tile_border_size{tile_size_f / 16};

  for (size_t y{0}; y < this->level->tilemap.get_height(); y += 1) {
    for (size_t x{0}; x < this->level->tilemap.get_width(); x += 1) {
      sf::RectangleShape rect(
          {tile_size_f - tile_border_size, tile_size_f - tile_border_size});

      rect.setPosition(
          static_cast<float>(x) * tile_size_f + (tile_border_size / 2),
          static_cast<float>(y) * tile_size_f + (tile_border_size / 2));

      if (this->level->tilemap.get_tile(x, y).has_value()) {
        rect.setFillColor(sf::Color::Black);
      } else {
        rect.setFillColor(sf::Color::White);
      }

      tilemap_tex.draw(rect);
    }
  }

  tilemap_tex.display();
}

TopdownView::TopdownView(const sf::Vector2f &size, const Level &level)
    : player_rect{{Player::SIZE, Player::SIZE}},
      player_line{{Player::SIZE, Player::SIZE / 2}}, level{&level} {

  const unsigned int tile_size{
      static_cast<unsigned int>(this->level->tile_size)};

  const unsigned int tilemap_width{
      static_cast<unsigned int>(this->level->tilemap.get_width())};

  const unsigned int tilemap_height{
      static_cast<unsigned int>(this->level->tilemap.get_height())};

  tilemap_tex.create(tilemap_width * tile_size, tilemap_height * tile_size);
  tilemap_sprite.setTexture(tilemap_tex.getTexture());
  draw_tilemap();
  player_rect.setOrigin(Player::SIZE / 2, Player::SIZE / 2);
  player_rect.setFillColor(sf::Color::Blue);
  player_line.setOrigin({0, player_line.getSize().y / 2});
  player_line.setFillColor(player_rect.getFillColor());
  update_player_position();
  update_player_rotation();
  view.setViewport(sf::FloatRect(0, 0, 0.5f, 1));
  update_size(size);
}

void TopdownView::update_size(const sf::Vector2f &size) {
  view.setSize(size.x / 2, size.y);
  view.zoom(0.5f);
}

void TopdownView::update_player_position() {
  const auto &player_pos{this->level->player.pos};
  player_rect.setPosition(player_pos);
  player_line.setPosition(player_pos);
  view.setCenter(this->level->player.pos.x, this->level->player.pos.y);

  rays_texture.create(static_cast<unsigned int>(view.getSize().x),
                      static_cast<unsigned int>(view.getSize().y));

  rays_sprite.setTexture(rays_texture.getTexture());
}

void TopdownView::update_player_rotation() {
  player_line.setRotation(this->level->player.get_rotation().get_degrees());
}

void TopdownView::clear_rays() { rays_texture.clear(sf::Color::Transparent); }

void TopdownView::cast_ray(const sf::Vector2f &origin, const float angle,
                           const float distance) {
  sf::RectangleShape line{{distance, 1}};
  line.setOrigin({0, line.getSize().y / 2});
  line.setPosition(origin);
  line.setRotation(Util::to_degrees(angle));
  line.setFillColor(sf::Color::Green);
  rays_texture.draw(line);
}

void TopdownView::draw(sf::RenderTarget *target) {
  target->draw(tilemap_sprite);
  rays_texture.display();
  target->draw(rays_sprite);
  target->draw(player_rect);
  target->draw(player_line);
}

const sf::View &TopdownView::get_view() const { return view; }

} // namespace Ray3d
