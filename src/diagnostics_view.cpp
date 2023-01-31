#include "diagnostics_view.hpp"
#include <cassert>
#include <cmath>

namespace Ray3d {

DiagnosticsView::DiagnosticsView(const sf::Vector2f &size,
                                 const sf::Font &font) {
  fps_text.setFont(font);
  fps_text.setCharacterSize(24);
  fps_text.setFillColor(sf::Color::White);
  fps_text.setOutlineColor(sf::Color::Black);
  fps_text.setOutlineThickness(1);
  update_fps(0);
  fps_text.setPosition(fps_text.getGlobalBounds().top, 0);
  update_size(size);
}

void DiagnosticsView::update_size(const sf::Vector2f &size) {
  view.setSize(size);
  view.setCenter(size.x / 2, size.y / 2);
}

void DiagnosticsView::draw(sf::RenderTarget *target) { target->draw(fps_text); }

void DiagnosticsView::update_fps(const float fps) {
  fps_text.setString("FPS: " +
                     std::to_string(static_cast<int>(std::round(fps))));
}

const sf::View &DiagnosticsView::get_view() const { return view; }

} // namespace Ray3d
