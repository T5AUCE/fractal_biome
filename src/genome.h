#pragma once
#include <SFML/Graphics.hpp>
#include <random>

struct FloatRange { float low, high; };
struct IntRange { int low, high; };

struct ShapeSpec {
    const char* name;
    int sides;
    FloatRange startSize;
    FloatRange startThick;
    FloatRange maxGrowthMult;
    IntRange   numChildren;
    IntRange   maxDepth;
    FloatRange childScale;
    FloatRange spreadFactor;
    FloatRange growthInterval;
    FloatRange thickRatio;
};

struct Genome {
    const ShapeSpec* spec = nullptr;
    float startSize = 0.f;
    float startThick = 0.f;
    float maxGrowthMult = 0.f;
    int   numChildren = 0;
    int   maxDepth = 0;
    float childScale = 0.f;
    float spreadFactor = 0.f;
    float growthInterval = 0.f;
    float thickRatio = 0.f;
    sf::Color innerColor;
    sf::Color outerColor;

    int sides() const { return spec->sides; }
};

std::mt19937& globalRng();
Genome rollGenome(const ShapeSpec& spec, std::mt19937& rng); 
Genome randomShapeGenome(std::mt19937& rng);