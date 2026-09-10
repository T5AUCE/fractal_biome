#include <SFML/Graphics.hpp>
#include <iostream>


int main()
{
    constexpr int window_width = 800;
    constexpr int window_length = 800;
    const float framerate = 60.0f;
    const float rows = 75.0f;
    const float columns = 75.0f;
    const float radius = 1.0f;
    const sf::Vector2f acceleration{ 40.0f,40.0f };

    float horizontal_increment = 0 - radius;
    float vertical_increment = 0 - radius;

    sf::Color color;
    sf::RenderWindow window(sf::VideoMode({ window_width,window_length }), "2d_sim_system");
    window.setFramerateLimit(framerate);


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
        window.display();
    }
}