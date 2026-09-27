#include <SFML/Graphics.hpp>
#include <iostream>
#include "render.h"
#include <random>

void expandFitView(sf::RenderWindow& window, float logicalWidth, float logicalHeight);

int main()
{
    std::mt19937 gen{ std::random_device{}() };
    constexpr int window_width = 1920;
    constexpr int window_height = 1080;
    const float framerate = 60.0f;

    const float size = 10.0f;
    const float pos_x = window_width / 2.f;
    const float pos_y = window_height / 2.f;
    const float thickness = 20.f;
    const int sides = 7;

    sf::RenderWindow window(sf::VideoMode({ window_width,window_height }), "fractal_biome");
    window.setFramerateLimit(framerate);
    bool isFullscreen = false;

    Render render(window);

    sf::Vector2f position{ pos_x,pos_y };

    std::uniform_int_distribution<int> dis(20, 255);

    sf::Color color(
        static_cast<std::uint8_t>(dis(gen)),
        static_cast<std::uint8_t>(dis(gen)),
        static_cast<std::uint8_t>(dis(gen))
    );
    sf::Color color2(
        static_cast<std::uint8_t>(dis(gen)),
        static_cast<std::uint8_t>(dis(gen)),
        static_cast<std::uint8_t>(dis(gen))
    );

    PolyOrg poly(position, color, color2, size, thickness,sides);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close();

            if (event->is<sf::Event::KeyPressed>()) {
                auto key = event->getIf<sf::Event::KeyPressed>();
                if (key && key->code == sf::Keyboard::Key::F11) {
                    isFullscreen = !isFullscreen;
                    if (isFullscreen) {
                        window.create(sf::VideoMode::getDesktopMode(), "fractal_biome", sf::State::Fullscreen);
                    }
                    else {
                        window.create(sf::VideoMode({ window_width, window_height }), "fractal_biome");
                    }
                    window.setFramerateLimit(framerate);
                    expandFitView(window, window_width, window_height);
                }
            }

            if (event->is<sf::Event::Resized>()) expandFitView(window, window_width, window_height);
        }
        window.clear();
        render.createRect(poly, window_height, window_width, framerate);
        window.display();
    }
}

void expandFitView(sf::RenderWindow& window, float designWidth, float designHeight) {
    sf::Vector2u windowSize = window.getSize();
    float windowRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
    float designRatio = designWidth / designHeight;

    float viewWidth, viewHeight;
    if (windowRatio > designRatio) {
        viewHeight = designHeight;
        viewWidth = designHeight * windowRatio;
    }
    else {
        viewWidth = designWidth;
        viewHeight = designWidth / windowRatio;
    }

    sf::View view({ designWidth / 2.f, designHeight / 2.f }, { viewWidth, viewHeight });
    window.setView(view);
}