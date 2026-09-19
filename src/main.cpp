#include <SFML/Graphics.hpp>
#include <iostream>
#include "render.h"


int main()
{
    constexpr int window_width = 1200;
    constexpr int window_length = 1000;
    const float framerate = 60.0f;
    const float size = 30.0f;
    const float pos_x = window_width / 2.f;
    const float pos_y = window_length / 2.f;
    const float thickness = 5.f;

    sf::RenderWindow window(sf::VideoMode({ window_width,window_length }), "fractal_biome");
    window.setFramerateLimit(framerate);

    Render render(window);

    sf::Vector2f position{ pos_x,pos_y };
    sf::Color color = sf::Color::Blue;
    RectOrg rect(position, color, size, thickness);

    /* Main Loop */
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        window.clear();
        render.createRect(rect, window_length, window_width, framerate);
        window.display();
    }
}