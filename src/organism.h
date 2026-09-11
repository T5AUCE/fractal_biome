#pragma once
#include <SFML/Graphics.hpp>

struct Organism {
	sf::Vector2f position;
	sf::Color color;
	float size = 30.0f;

	Organism(sf::Vector2f pos, sf::Color col, float sz) :
		position{pos},
		color{col},
		size{sz}
	{}

	void update() {
	}
};