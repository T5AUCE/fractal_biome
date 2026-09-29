#pragma once
#include <SFML/Graphics.hpp>
#include "camera.h"

class InputHandler {
public:
    void handleEvent(const sf::Event& event, sf::RenderWindow& window, Camera& camera,
        float designWidth, float designHeight);

private:
    bool dragging_ = false;
    sf::Vector2i lastMousePixel_;
};