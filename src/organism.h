#pragma once
#include <SFML/Graphics.hpp>
#include <numbers>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstdint>
#include "genome.h"
#include <functional>

struct Organism {
	Genome genome;
	sf::Vector2f position;
	float size;
	float thick;
	int depth = 0;
	float age = 0.f;

	std::vector<std::unique_ptr<Organism>> children;

	Organism(sf::Vector2f pos, const Genome& g, float sz, float thc, int d = 0)
		: genome{ g }, position{ pos }, size{ sz }, thick{ thc }, depth{ d }
	{
	}

	virtual void update(float dt) {
		age += dt;
		growSelf();
		if (children.empty() && age >= genome.growthInterval && depth < genome.maxDepth)
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
	sf::Color outerColor)
{
	sf::VertexArray va(sf::PrimitiveType::TriangleStrip, (sides + 1) * 2);
	const float innerRadius = std::max(0.f, outerRadius - thickness);
	constexpr float twoPi = 2.f * std::numbers::pi_v<float>;

	for (int i = 0; i <= sides; ++i) {
		float angle = twoPi * i / sides;
		sf::Vector2f dir{ std::cos(angle), std::sin(angle) };

		va[i * 2 + 0].position = center + dir * outerRadius;
		va[i * 2 + 0].color    = outerColor;
		va[i * 2 + 1].position = center + dir * innerRadius;
		va[i * 2 + 1].color    = innerColor;
	}
	return va;
}

struct PolyOrg : public Organism {
	sf::VertexArray shape;
	sf::Color innerColor, outerColor;

	PolyOrg(sf::Vector2f pos, const Genome& g, float sz, float thc, int d = 0)
		: Organism(pos, g, sz, thc, d),
		innerColor{ g.innerColor }, outerColor{ g.outerColor }
	{
		rebuildShape();
	}

	void rebuildShape() {
		shape = makeGradientPolygon(position, size / 2.f, thick,
			genome.sides(), innerColor, outerColor);
	}

	void growSelf() override {
		float growthFactor = 1.f + 0.0005f / (1.f + depth * 0.5f);
		size *= growthFactor;
		thick *= growthFactor;
		rebuildShape();
	}

	void replicate() override {
		const float childSize = size * genome.childScale;
		const float distance = size * genome.spreadFactor;
		const int n = genome.numChildren;

		for (int i = 0; i < n; ++i) {
			float rad = (360.f / n) * i * std::numbers::pi / 180.f;
			sf::Vector2f offset{ std::cos(rad) * distance, std::sin(rad) * distance };

			auto child = std::make_unique<PolyOrg>(
				position + offset, genome, childSize, thick * genome.thickRatio, depth + 1);
			children.push_back(std::move(child));
		}
	}

	void draw(sf::RenderTarget& target) override {
		target.draw(shape);
		Organism::draw(target);
	}
};