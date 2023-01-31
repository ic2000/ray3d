#include "engine.hpp"
#include "raycasting.hpp"
#include "util.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

namespace Ray3d {

void Engine::load_win_settings() {
  win_params.width = 1600;
  win_params.height = 800;
  win_params.title = "Ray3d";
  win_params.fullscreen = false;
  win_params.framerate_limit = 165;
  win_params.vertical_sync = false;
}

void Engine::load_fonts() {
  const std::string_view fonts_dir{"assets/fonts/"};
  assert(std::filesystem::exists(fonts_dir));

  for (const auto &entry : std::filesystem::directory_iterator(fonts_dir)) {
    sf::Font font{};
    assert(font.loadFromFile(entry.path()));
    const auto &name{std::string(entry.path()).substr(fonts_dir.size())};
    fonts[name] = font;
  }
}

void Engine::load_level(std::string_view level_name) {
  rapidjson::Document doc{};

  {
    const std::string level_path{"assets/levels/" + std::string(level_name)};
    std::ifstream ifs{level_path};
    assert(ifs.is_open());
    rapidjson::IStreamWrapper isw{ifs};
    doc.ParseStream(isw);
  }

  assert(doc.IsObject());
  current_level = std::make_unique<Level>(doc);
  views.erase(topdown_view.get());

  topdown_view = std::make_unique<TopdownView>(
      sf::Vector2f{static_cast<float>(win_params.width),
                   static_cast<float>(win_params.height)},
      *current_level.get());

  views.emplace(topdown_view.get());
}

Engine::Engine() {
  load_fonts();
  load_win_settings();
  const auto &diagnostics_font_it{fonts.find(DIAGNOSTICS_FONT)};
  assert(diagnostics_font_it != fonts.end());

  diagnostics_view = std::make_unique<DiagnosticsView>(
      sf::Vector2f{static_cast<float>(win_params.width),
                   static_cast<float>(win_params.height)},
      diagnostics_font_it->second);

  views.emplace(diagnostics_view.get());

  game_view = std::make_unique<GameView>(
      sf::Vector2f{static_cast<float>(win_params.width),
                   static_cast<float>(win_params.height)});

  views.emplace(game_view.get());
}

void Engine::process_events() {
  sf::Event event{};

  while (win->pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      win->close();
    } else if (event.type == sf::Event::KeyPressed) {
      keys.insert(event.key.code);
    } else if (event.type == sf::Event::KeyReleased) {
      keys.erase(event.key.code);
    } else if (event.type == sf::Event::Resized) {
      for (auto &view : views)
        view->update_size({static_cast<float>(event.size.width),
                           static_cast<float>(event.size.height)});

      cast_rays();
    }
  }
}

void Engine::create_window() {
  win = std::make_unique<sf::RenderWindow>(
      sf::VideoMode{win_params.width, win_params.height}, win_params.title,
      win_params.fullscreen ? sf::Style::Fullscreen : sf::Style::Default);

  if (win_params.framerate_limit > 0)
    win->setFramerateLimit(win_params.framerate_limit);

  win->setVerticalSyncEnabled(win_params.vertical_sync);
  win->setActive(true);
}

bool Engine::handle_player_movement(const float elapsed_time) {
  const float movement_speed{PLAYER_MOVEMENT_SPEED * elapsed_time};
  const float strafe_speed{PLAYER_STRAFE_SPEED * elapsed_time};
  bool moved{false};

  if (keys.contains(sf::Keyboard::Key::W)) {
    player->move_forward(movement_speed);
    moved |= true;
  }

  if (keys.contains(sf::Keyboard::Key::S)) {
    player->move_backward(movement_speed);
    moved |= true;
  }

  if (keys.contains(sf::Keyboard::Key::A)) {
    player->move_left(strafe_speed);
    moved |= true;
  }

  if (keys.contains(sf::Keyboard::Key::D)) {
    player->move_right(strafe_speed);
    moved |= true;
  }

  return moved;
}

bool Engine::handle_player_rotation(const float elapsed_time) {
  const float rotation_speed{PLAYER_ROTATION_SPEED * elapsed_time};
  bool rotated{false};

  if (keys.contains(sf::Keyboard::Key::Right)) {
    player->rotate(rotation_speed);
    rotated |= true;
  }

  if (keys.contains(sf::Keyboard::Key::Left)) {
    player->rotate(-rotation_speed);
    rotated |= true;
  }

  return rotated;
}

void Engine::cast_ray(const int index, const sf::Vector2f origin,
                      const float angle) {
  const auto horizontal_intersection(
      Raycasting::cast_horizontal_ray(origin, angle, *current_level));

  const auto vertical_intersection(
      Raycasting::cast_vertical_ray(origin, angle, *current_level));

  if ((horizontal_intersection.x == -1 || horizontal_intersection.y == -1) &&
      (vertical_intersection.x == -1 || vertical_intersection.y == -1)) {
    return;
  }

  const auto [intersection, distance]{Raycasting::closest_ray_intersection(
      origin, horizontal_intersection, vertical_intersection)};

  topdown_view->cast_ray(origin, angle, distance);

  RayDirection ray_direction{&intersection == &horizontal_intersection
                                 ? RayDirection::HORIZONTAL
                                 : RayDirection::VERTICAL};

  game_view->cast_ray(index, player->get_rotation().get_radians(), angle,
                      distance, PLAYER_FOV, current_level->tile_size,
                      ray_direction);
}

void Engine::cast_player_rays() {
  topdown_view->clear_rays();
  Angle angle{player->get_rotation().get_radians() -
              Util::to_radians(static_cast<float>(PLAYER_FOV) / 2)};

  angle -= 0.0001f;

  for (int i{0}; i < PLAYER_FOV; i += 1) {
    cast_ray(i, player->pos, angle.get_radians());
    angle += Util::to_radians(1);
  }
}

void Engine::cast_rays() {
  topdown_view->clear_rays();
  game_view->clear_rays();
  cast_player_rays();
}

void Engine::update(const float elapsed_time) {
  static bool first_update{true};
  bool call_cast_rays{false};

  diagnostics_view->update_fps(1.f / elapsed_time);

  if (handle_player_movement(elapsed_time)) {
    topdown_view->update_player_position();
    call_cast_rays = true;
  }

  if (handle_player_rotation(elapsed_time)) {
    topdown_view->update_player_rotation();
    call_cast_rays = true;
  }

  if (first_update || call_cast_rays)
    cast_rays();

  first_update = false;
}

void Engine::draw_view(View *view) {
  win->setView(view->get_view());
  view->draw(win.get());
}

void Engine::render() {
  win->clear(sf::Color(78, 78, 78));
  draw_view(topdown_view.get());
  draw_view(game_view.get());
  draw_view(diagnostics_view.get());
  win->display();
}

void Engine::run() {
  load_level("demo.json");
  player = &current_level->player;
  create_window();
  sf::Clock clock{};

  while (win->isOpen()) {
    const auto elapsed_time{clock.getElapsedTime().asSeconds()};
    clock.restart();
    process_events();
    update(elapsed_time);
    render();
  }
}

} // namespace Ray3d
