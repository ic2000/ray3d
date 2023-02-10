#include "level.hpp"
#include "util.hpp"
#include <SFML/Graphics/Color.hpp>
#include <string_view>

static sf::Color parse_colour(const rapidjson::Value &color) {
  assert(color.IsArray());
  const auto &color_array{color.GetArray()};
  assert(color_array.Size() == 4);
  assert(color_array[0].IsInt());
  assert(color_array[1].IsInt());
  assert(color_array[2].IsInt());
  assert(color_array[3].IsInt());

  return sf::Color{static_cast<sf::Uint8>(color_array[0].GetInt()),
                   static_cast<sf::Uint8>(color_array[1].GetInt()),
                   static_cast<sf::Uint8>(color_array[2].GetInt()),
                   static_cast<sf::Uint8>(color_array[3].GetInt())};
}

namespace Ray3d {

Level::Level(const rapidjson::Document &doc) {
  auto parse_tile_size{[this, &doc]() {
    assert(doc.HasMember("tile_size"));
    assert(doc["tile_size"].IsInt());
    tile_size = doc["tile_size"].GetInt();
  }};

  auto parse_tilemap{[this, &doc]() {
    assert(doc.HasMember("tilemap"));
    assert(doc["tilemap"].IsArray());
    const auto &tilemap_array = doc["tilemap"].GetArray();
    assert(tilemap_array.Size() > 0);
    const auto tilemap_array_size = tilemap_array.Size();
    const auto &tilemap_array_first = tilemap_array[0].GetArray();
    assert(tilemap_array_first.Size() > 0);
    tilemap = Tilemap(tilemap_array_first.Size(), tilemap_array_size);
    assert(doc.HasMember("tiles"));
    assert(doc["tiles"].IsObject());
    const auto &tiles{doc["tiles"]};

    for (decltype(tilemap_array_first.Size()) y{0}; y < tilemap_array_size;
         y += 1) {
      const auto tilemap_row = tilemap_array[y].GetArray();
      assert(tilemap_row.Size() == tilemap_array_first.Size());

      for (decltype(y) x{0}; x < tilemap_row.Size(); x += 1) {
        assert(tilemap_row[x].IsInt());
        const auto &tile_key{std::to_string(tilemap_row[x].GetInt())};
        assert(tiles.HasMember(tile_key.c_str()));
        const auto &tile_obj{tiles[tile_key.c_str()]};
        assert(tile_obj.IsObject());
        assert(tile_obj.HasMember("height"));
        assert(tile_obj["height"].IsNumber());
        assert(tile_obj.HasMember("floor_color"));
        const auto floor_color{parse_colour(tile_obj["floor_color"])};
        assert(tile_obj.HasMember("wall_color"));
        const auto wall_color{parse_colour(tile_obj["wall_color"])};

        tilemap.set_tile(
            x, y, {tile_obj["height"].GetFloat(), floor_color, wall_color});
      }
    }
  }};

  auto parse_player{[this, &doc]() {
    assert(doc.HasMember("player"));
    const auto &player_obj{doc["player"]};
    assert(player_obj.IsObject());
    assert(player_obj.HasMember("x"));
    assert(player_obj["x"].IsNumber());
    player.pos.x = player_obj["x"].GetFloat();
    assert(player_obj.HasMember("y"));
    assert(player_obj["y"].IsNumber());
    player.pos.y = player_obj["y"].GetFloat();
    assert(player_obj.HasMember("angle"));
    assert(player_obj["angle"].IsNumber());

    player.set_rotation(
        Angle(Util::to_radians(player_obj["angle"].GetFloat())));
  }};

  // auto parse_entities{[this, &doc]() {
  // assert(doc.HasMember("entities"));
  // assert(doc["entities"].IsArray());

  // for (const auto &entity : doc["entities"].GetArray()) {
  //   assert(entity.HasMember("type"));
  //   assert(entity["type"].IsString());
  //   assert(entity.HasMember("x"));
  //   assert(entity["x"].IsInt());
  //   assert(entity.HasMember("y"));
  //   assert(entity["y"].IsInt());

  // entities.emplace(entity["type"].GetString(),
  //                  Entity{entity["type"].GetString(),
  //                  entity["x"].GetInt(),
  //                         entity["y"].GetInt()});
  //   }
  // }};

  parse_tile_size();
  parse_tilemap();
  parse_player();
  // parse_entities();
}

// const std::unordered_map<std::string, Entity> &Level::get_entities() const {
//   return entities;
// }

} // namespace Ray3d
