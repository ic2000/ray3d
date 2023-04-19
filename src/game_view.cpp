#include "game_view.hpp"
#include "constants.hpp"
#include "tile.hpp"
#include "util.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <iostream>

namespace Ray3d {

void GameView::draw_crosshair() {
  crosshair_tex.create(CROSSHAIR_SIZE, CROSSHAIR_SIZE);
  crosshair_tex.clear(sf::Color::Transparent);
  sf::RectangleShape crosshair_rect{{CROSSHAIR_SIZE, CROSSHAIR_SIZE / 8.f}};

  crosshair_rect.setOrigin(crosshair_rect.getLocalBounds().width / 2,
                           crosshair_rect.getLocalBounds().height / 2);

  crosshair_rect.setFillColor(sf::Color::White);

  crosshair_rect.setPosition(static_cast<float>(crosshair_tex.getSize().x) / 2,
                             static_cast<float>(crosshair_tex.getSize().y) / 2);

  crosshair_tex.draw(crosshair_rect);
  crosshair_rect.rotate(90);
  crosshair_tex.draw(crosshair_rect);
  crosshair_tex.display();
  crosshair_sprite.setTexture(crosshair_tex.getTexture());

  crosshair_sprite.setOrigin(crosshair_sprite.getLocalBounds().width / 2,
                             crosshair_sprite.getLocalBounds().height / 2);
}

GameView::GameView(const sf::Vector2f &size) {
  draw_crosshair();
  view.setViewport({0.5f, 0, 0.5f, 1});
  update_size(size);
}

sf::Color multiply_colour(const sf::Color &colour, const float factor) {
  sf::Color new_colour{
      std::min<sf::Uint8>(static_cast<sf::Uint8>(colour.r * factor), 255),
      std::min<sf::Uint8>(static_cast<sf::Uint8>(colour.g * factor), 255),
      std::min<sf::Uint8>(static_cast<sf::Uint8>(colour.b * factor), 255),
      colour.a};

  return new_colour;
}

void GameView::update_size(const sf::Vector2f &size) {
  view.setSize(size.x / 2, size.y);
  view.setCenter(view.getSize() / 2.f);

  hud_tex.create(static_cast<unsigned int>(view.getSize().x),
                 static_cast<unsigned int>(view.getSize().y));

  hud_sprite = sf::Sprite(hud_tex.getTexture());
  crosshair_sprite.setPosition(view.getSize().x / 2, view.getSize().y / 2);

  sky =
      sf::RectangleShape{sf::Vector2f{view.getSize().x, view.getSize().y / 2}};
  sky.setFillColor(multiply_colour(sf::Color::Blue, 0.5f));

  floor =
      sf::RectangleShape{sf::Vector2f{view.getSize().x, view.getSize().y / 2}};

  floor.setFillColor(multiply_colour(sf::Color::Magenta, 0.25f));
  floor.setPosition(0, view.getSize().y / 2);

  render_tex.create(static_cast<unsigned int>(view.getSize().x),
                    static_cast<unsigned int>(view.getSize().y));

  render_sprite = sf::Sprite(render_tex.getTexture());
}

void GameView::clear_rays() { render_tex.clear(sf::Color::Transparent); }

float GameView::cast_ray(const int index, const float origin_angle,
                         const float ray_angle, float distance, const int fov,
                         const int tile_size, RayDirection ray_direction,
                         const Tile &tile, float current_height) {
  if (distance <= 0)
    distance += 0.001f;

  const auto ca{Util::modulo(origin_angle - ray_angle, Constants::TAU)};
  distance *= std::cos(ca);

  const auto tex_width_height_ratio{static_cast<float>(render_tex.getSize().x) /
                                    static_cast<float>(render_tex.getSize().y)};

  const auto projected_height{std::min(
      (tile_size / distance) * render_tex.getSize().y * tex_width_height_ratio,
      static_cast<float>(render_tex.getSize().y))};

  const auto line_width{static_cast<float>(render_tex.getSize().x) / fov};

  auto line{sf::RectangleShape{
      sf::Vector2f{line_width, projected_height * tile.height}}};

  line.setPosition(static_cast<float>(index) * line_width,
                   static_cast<float>(render_tex.getSize().y) / 2 -
                       -projected_height / 2 - line.getSize().y);

  // if (tile.wall_color == sf::Color::White) {
  //   current_height = 0;
  // }

  auto line2 = line;

  if (current_height == -1) {
    current_height = line2.getPosition().y;
  } else {
    if (line2.getPosition().y >= current_height)
      return current_height;

    auto line_remainder{current_height -
                        (line2.getPosition().y + line2.getSize().y)};

    auto line2_height{line2.getSize().y};

    if (line_remainder < 0) {
      std::cout << line_remainder << std::endl;
      line2_height += line_remainder;
    }

    if (line2_height <= 0)
      return current_height;

    line2.setSize({line2.getSize().x, line2_height});
    line2.setPosition(line.getPosition().x, line.getPosition().y);
  }

  auto circle = sf::CircleShape{5};
  circle.setPosition(line2.getPosition().x, line2.getPosition().y);
  auto circle2 = sf::CircleShape{5};

  circle2.setPosition(line2.getPosition().x + line2.getSize().x,
                      line2.getPosition().y + line2.getSize().y);

  line.setFillColor(tile.wall_color);

  // line.setFillColor(
  //     sf::Color(tile.wall_color.r, tile.wall_color.g, tile.wall_color.b, 200));

  if (ray_direction == RayDirection::VERTICAL)
    line.setFillColor(multiply_colour(line.getFillColor(), 1.5f));

  line.setFillColor(multiply_colour(line.getFillColor(),
                                    std::min(FOG_DISTANCE / distance, 1.f)));

  line2.setFillColor(line.getFillColor());

  // render_tex.draw(line);
  render_tex.draw(line2);
  // render_tex.draw(circle);
  // circle2.setFillColor(sf::Color::Red);
  // render_tex.draw(circle2);

  return line2.getPosition().y;
}

void GameView::draw(sf::RenderTarget *target) {
  target->draw(sky);
  target->draw(floor);
  render_tex.display();
  target->draw(render_sprite);
  hud_tex.clear(sf::Color::Transparent);
  hud_tex.draw(crosshair_sprite);
  hud_tex.display();
  target->draw(hud_sprite);
}

const sf::View &GameView::get_view() const { return view; }

} // namespace Ray3d
