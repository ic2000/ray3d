#pragma once

#include "diagnostics_view.hpp"
#include "game_view.hpp"
#include "level.hpp"
#include "player.hpp"
#include "topdown_view.hpp"
#include "view.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

namespace Ray3d {

struct WinParams {
  unsigned int width;
  unsigned int height;
  std::string title;
  bool fullscreen;
  unsigned int framerate_limit;
  bool vertical_sync;
};

class Engine {
  static constexpr char DIAGNOSTICS_FONT[]{"calibri_regular.ttf"};
  static constexpr float PLAYER_MOVEMENT_SPEED{25};
  static constexpr float PLAYER_STRAFE_SPEED{PLAYER_MOVEMENT_SPEED * 0.75f};
  static constexpr float PLAYER_ROTATION_SPEED{3};
  static constexpr int PLAYER_FOV{60};

  std::unordered_map<std::string, sf::Font> fonts{};
  WinParams win_params{};
  std::unordered_set<View *> views{};
  std::unique_ptr<DiagnosticsView> diagnostics_view;
  std::unique_ptr<GameView> game_view;
  std::unique_ptr<Level> current_level;
  Player *player{};
  std::unique_ptr<TopdownView> topdown_view;
  std::unique_ptr<sf::RenderWindow> win{};
  std::unordered_set<sf::Keyboard::Key> keys{};

  void load_fonts();
  void load_win_settings();
  void load_level(std::string_view level_name);
  void create_window();
  void process_events();
  bool handle_player_movement(const float elapsed_time);
  bool handle_player_rotation(const float elapsed_time);

  void cast_ray(const int index, const sf::Vector2f origin, const float angle,
                float line_height = -1);

  void cast_player_rays();
  void cast_rays();
  void update(const float elapsed_time);
  void draw_view(View *view);
  void render();

public:
  Engine();
  void run();
};

} // namespace Ray3d
