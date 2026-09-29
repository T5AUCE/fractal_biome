#pragma once
#include <SFML/Graphics.hpp>

struct Camera {
    sf::Vector2f center;
    float zoom = 1.f;

    void clampZoom(float minZoom, float maxZoom) {
        zoom = std::clamp(zoom, minZoom, maxZoom);
    }
};

void applyView(sf::RenderWindow& window, const Camera& cam, float designWidth, float designHeight);

void zoomAt(sf::RenderWindow& window, Camera& cam, sf::Vector2i pixel, float factor,
    float designWidth, float designHeight);