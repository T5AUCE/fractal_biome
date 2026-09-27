#pragma once
#include <SFML/Graphics.hpp>
#include <numbers>
#include <random>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstdint>
#include <functional>

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
	static constexpr float spreadFactor = 2.0f;

	std::vector<std::unique_ptr<Organism>> children;

	Organism(sf::Vector2f pos, sf::Color col, float sz, float thc, int d = 0)
		: position{ pos }, color{ col }, size{ sz }, thick{ thc }, depth{ d }
	{
	}

	virtual void update(float dt) {
		age += dt;
		if (age <= 50 - ((depth + 1) * growthInterval))
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

inline sf::VertexArray makeGradientPolygon(
	sf::Vector2f center,
	float outerRadius,
	float thickness,
	int sides,
	sf::Color innerColor,
	sf::Color outerColor,
	std::function<sf::Color(float angleRad)> angleColorFn = nullptr)
{
	sf::VertexArray va(sf::PrimitiveType::TriangleStrip, (sides + 1) * 2);
	float innerRadius = std::max(0.f, outerRadius - thickness);

	for (int i = 0; i <= sides; ++i) {
		float angle = (360.f / sides) * i * std::numbers::pi / 180.f;
		sf::Vector2f dir{ std::cos(angle), std::sin(angle) };

		sf::Color outerC = outerColor;
		sf::Color innerC = innerColor;

		if (angleColorFn) {
			sf::Color angleC = angleColorFn(angle);
			outerC = sf::Color(
				(outerC.r + angleC.r) / 2, (outerC.g + angleC.g) / 2,
				(outerC.b + angleC.b) / 2, outerC.a);
			innerC = sf::Color(
				(innerC.r + angleC.r) / 2, (innerC.g + angleC.g) / 2,
				(innerC.b + angleC.b) / 2, innerC.a);
		}

		va[i * 2 + 0].position = center + dir * outerRadius;
		va[i * 2 + 0].color = outerC;
		va[i * 2 + 1].position = center + dir * innerRadius;
		va[i * 2 + 1].color = innerC;
	}
	return va;
}

struct RectOrg : public Organism {
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
				clampByte(color.g + (numChildren - i - 1) * 20 * dis(gen)),
				clampByte(color.b - i * 5 * dis(gen))
			);

			children.push_back(std::make_unique<RectOrg>(
				position + offset, newCol, childSize, thick * 0.6f, depth + 1));
		}
	}

	void draw(sf::RenderTarget& target) override {
		target.draw(rect);
		Organism::draw(target);
	}
};

struct PolyOrg : public Organism {
	sf::VertexArray shape;
	int sides;
	sf::Color innerColor, outerColor;
	static inline std::mt19937 gen{ std::random_device{}() };

	PolyOrg(sf::Vector2f pos, sf::Color inner, sf::Color outer, float sz, float thc, int sidesCount, int d = 0)
		: Organism(pos, outer, sz, thc, d), sides{ sidesCount }, innerColor{ inner }, outerColor{ outer }
	{
		shape = makeGradientPolygon(position, size / 2.f, thick, sides, innerColor, outerColor);
	}

	void growSelf() override {
		size *= 1.0005f;
		thick *= 1.0005f;
		shape = makeGradientPolygon(position, size / 2.f, thick, sides, innerColor, outerColor);
	}

	void draw(sf::RenderTarget& target) override {
		target.draw(shape);
		Organism::draw(target);
	}

	void replicate() override {
		const int numChildren = sides;
		const float childSize = size * childScale;
		const float distance = size * spreadFactor;

		for (int i = 0; i < numChildren; ++i) {
			float rad = (360.f / numChildren) * i * std::numbers::pi / 180.f;
			sf::Vector2f offset{ std::cos(rad) * distance, std::sin(rad) * distance };

			std::uniform_real_distribution<double> dis(0.0, 2.0);
			auto clampByte = [](double v) -> std::uint8_t {
				return static_cast<std::uint8_t>(std::clamp(v, 0.0, 255.0));
			};

			sf::Color newOut(
				clampByte(outerColor.r + i * 20 * dis(gen)),
				clampByte(outerColor.g + (numChildren - i - 1) * 20 * dis(gen)),
				clampByte(outerColor.b - i * 5 * dis(gen))
			);

			sf::Color newIn = innerColor;

			children.push_back(std::make_unique<PolyOrg>(position + offset, newIn, newOut, childSize, thick * 0.6f, sides, depth + 1));
		}
	}
};