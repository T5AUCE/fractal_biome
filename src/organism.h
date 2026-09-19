#pragma once
#include <SFML/Graphics.hpp>

struct Organism {
	sf::Vector2f position;
	sf::Color color;
	float size;

	Organism(sf::Vector2f pos, sf::Color col, float sz) :
		position{pos},
		color{col},
		size{sz}
	{}

	virtual void update() {}

	virtual void draw(sf::RenderTarget& target) {}

	virtual ~Organism() = default;
};

struct RectOrg: public Organism {
	sf::RectangleShape rect;

	RectOrg(sf::Vector2f pos, sf::Color col, float sz)
		: Organism(pos,col,sz)
	{
		rect.setSize({ size, size });
		rect.setOrigin({ size / 2.f, size / 2.f });
		rect.setPosition(position);
		rect.setOutlineColor(color);
		rect.setOutlineThickness(5.f);
		rect.setFillColor(sf::Color::Transparent);
	}

	void update() override {
		size *= 1.001f;
		rect.setSize({ size, size });
	}

	void draw(sf::RenderTarget& target) override {
		target.draw(rect);
	}
};