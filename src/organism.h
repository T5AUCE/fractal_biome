#pragma once
#include <SFML/Graphics.hpp>
#include  <numbers>
#include <random>

struct Organism {
	sf::Vector2f position;
	sf::Color color;
	float size;
	float thick;
	int depth = 0;
	float age = 0.f;

	static constexpr float growthInterval = 10.0f; 
	static constexpr int   maxDepth = 4;
	static constexpr float childScale = 0.45f;
	static constexpr float spreadFactor = 2.2f; 

	std::vector<std::unique_ptr<Organism>> children;

	Organism(sf::Vector2f pos, sf::Color col, float sz, float thc, int d = 0 )
		: position{ pos }, color{ col }, size{ sz }, thick{ thc }, depth{ d }
	{}

	virtual void update(float dt) {
		age += dt;
		growSelf();
		if (children.empty() && age >= growthInterval && depth < maxDepth)
			replicate();
		for (auto& child : children)
			child->update(dt);
	}

	virtual void draw(sf::RenderTarget& target) {
		for (auto& child : children)
			child->draw(target);
	}

	virtual void growSelf() {}    
	virtual void replicate() {}

	virtual ~Organism() = default;
};

struct RectOrg: public Organism {
	sf::RectangleShape rect;
	static inline std::mt19937 gen{ std::random_device{}() };


	RectOrg(sf::Vector2f pos, sf::Color col, float sz, float thc, int d = 0)
		: Organism(pos, col, sz, thc, d)
	{
		rect.setSize({ size, size });
		rect.setOrigin({ size / 2.f, size / 2.f });
		rect.setPosition(position);
		rect.setOutlineColor(color);
		rect.setOutlineThickness(thc);
		rect.setFillColor(sf::Color::Transparent);
	}

	void growSelf() override {
		size *= 1.0005f;
		thick *= 1.0005f;
		rect.setSize({ size, size });
		rect.setOutlineThickness(thick);
		rect.setOrigin({ size / 2.f, size / 2.f });
		rect.setPosition(position);
	}

	void replicate() override {
		const int numChildren = 4;
		const float childSize = size * childScale;
		const float distance = size * spreadFactor;

		for (int i = 0; i < numChildren; ++i) {
			float rad = (360.f / numChildren) * i * std::numbers::pi / 180.f;
			sf::Vector2f offset{ std::cos(rad) * distance, std::sin(rad) * distance };

			std::uniform_real_distribution<double> dis(0.0, 2.0);

			auto clampByte = [](double v) -> std::uint8_t {
				return static_cast<std::uint8_t>(std::clamp(v, 0.0, 255.0));
			};

			sf::Color newCol(
				clampByte(color.r + i * 20 * dis(gen)),
				clampByte(color.g + (numChildren - i) * 20 * dis(gen)),
				clampByte(color.b - i * 15 * dis(gen))
			);

			children.push_back(std::make_unique<RectOrg>(position + offset, newCol, childSize, thick * 0.6f, depth + 1));
		}
	}

	void draw(sf::RenderTarget& target) override {
		target.draw(rect);
		Organism::draw(target); 
	}
};

