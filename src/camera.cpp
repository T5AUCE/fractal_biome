#include "camera.h"
#include <algorithm>

void applyView(sf::RenderWindow& window, const Camera& cam, float designWidth, float designHeight) {
    sf::Vector2u windowSize = window.getSize();
    float windowRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
    float designRatio = designWidth / designHeight;

    float viewWidth, viewHeight;
    if (windowRatio > designRatio) {
        viewHeight = designHeight;
        viewWidth = designHeight * windowRatio;
    } else {
        viewWidth = designWidth;
        viewHeight = designWidth / windowRatio;
    }

    sf::View view(cam.center, { viewWidth * cam.zoom, viewHeight * cam.zoom });
    window.setView(view);
}

void zoomAt(sf::RenderWindow& window, Camera& cam, sf::Vector2i pixel, float factor,
            float designWidth, float designHeight) {
    sf::Vector2f beforeWorld = window.mapPixelToCoords(pixel);

    cam.zoom *= factor;
    cam.clampZoom(0.1f, 5.f);
    applyView(window, cam, designWidth, designHeight);

    sf::Vector2f afterWorld = window.mapPixelToCoords(pixel);
    cam.center += beforeWorld - afterWorld;
    applyView(window, cam, designWidth, designHeight);
}