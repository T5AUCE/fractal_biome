#include <SFML/Graphics.hpp>
#include <iostream>
#include "render.h"
#include "genome.h"
#include "camera.h"
#include "input.h"

int main()
{
    constexpr float window_width = 1920;
    constexpr float window_height = 1080;
    const float framerate = 60.0f;

    const float pos_x = window_width / 2.f;
    const float pos_y = window_height / 2.f;

    sf::RenderWindow window(sf::VideoMode({ 1920u, 1080u }), "fractal_biome");
    window.setFramerateLimit(framerate);
    bool isFullscreen = false;

    Camera camera{ { pos_x, pos_y }, 1.f };
    InputHandler input;
    applyView(window, camera, window_width, window_height);

    Render render(window);

    sf::Vector2f position{ pos_x, pos_y };
    Genome g = randomShapeGenome(globalRng());
    PolyOrg poly(position, g, g.startSize, g.startThick);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                continue;
            }

            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::F11) {
                    isFullscreen = !isFullscreen;
                    if (isFullscreen) {
                        window.create(sf::VideoMode::getDesktopMode(), "fractal_biome", sf::State::Fullscreen);
                    }
                    else {
                        window.create(sf::VideoMode({ 1920u, 1080u }), "fractal_biome");
                    }
                    window.setFramerateLimit(framerate);
                    applyView(window, camera, window_width, window_height);
                    continue;
                }
            }

            input.handleEvent(*event, window, camera, window_width, window_height);
        }
        window.clear();
        render.createOrg(poly, window_height, window_width, framerate);
        window.display();
    }
}