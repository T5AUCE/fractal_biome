#include "input.h"

void InputHandler::handleEvent(const sf::Event& event, sf::RenderWindow& window, Camera& camera,
    float designWidth, float designHeight) {
    if (event.is<sf::Event::Resized>()) {
        applyView(window, camera, designWidth, designHeight);
        return;
    }

    if (const auto* scroll = event.getIf<sf::Event::MouseWheelScrolled>()) {
        float factor = (scroll->delta > 0) ? 0.9f : 1.1f;
        zoomAt(window, camera, { scroll->position.x, scroll->position.y }, factor,
            designWidth, designHeight);
        return;
    }

    if (const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (pressed->button == sf::Mouse::Button::Left) {
            dragging_ = true;
            lastMousePixel_ = { pressed->position.x, pressed->position.y };
        }
        return;
    }

    if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (released->button == sf::Mouse::Button::Left)
            dragging_ = false;
        return;
    }

    if (const auto* moved = event.getIf<sf::Event::MouseMoved>()) {
        if (dragging_) {
            sf::Vector2i currentPixel{ moved->position.x, moved->position.y };
            sf::Vector2f before = window.mapPixelToCoords(lastMousePixel_);
            sf::Vector2f after = window.mapPixelToCoords(currentPixel);
            camera.center += before - after;
            applyView(window, camera, designWidth, designHeight);
            lastMousePixel_ = currentPixel;
        }
        return;
    }
}