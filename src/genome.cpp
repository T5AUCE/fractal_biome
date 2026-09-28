#include "genome.h"
#include <cstdint>
#include <iterator>

namespace {
    const ShapeSpec kShapes[] = {
    {
        .name = "triangle",
        .sides = 3,
        .startSize = {10.f, 24.f},
        .startThick = {2.f, 4.f},
        .numChildren = {3, 3},
        .maxDepth = {4, 5},
        .childScale = {0.40f, 0.55f},
        .spreadFactor = {1.6f, 2.4f},
        .growthInterval = {6.f, 12.f},
        .thickRatio = {0.5f, 0.7f},
    },
    {
        .name = "square",
        .sides = 4,
        .startSize = {10.f, 24.f},
        .startThick = {2.f, 4.f},
        .numChildren = {4, 4},
        .maxDepth = {3, 4},
        .childScale = {0.35f, 0.50f},
        .spreadFactor = {1.8f, 2.6f},
        .growthInterval = {8.f, 14.f},
        .thickRatio = {0.5f, 0.7f},
    },
    {
        .name = "hexagon",
        .sides = 6,
        .startSize = {10.f, 24.f},
        .startThick = {2.f, 4.f},
        .numChildren = {6, 6},
        .maxDepth = {3, 4},
        .childScale = {0.30f, 0.45f},
        .spreadFactor = {2.0f, 3.0f},
        .growthInterval = {8.f, 16.f},
        .thickRatio = {0.5f, 0.7f},
    },
    };

    float rollFloat(FloatRange r, std::mt19937& rng) {
        return std::uniform_real_distribution<float>(r.low, r.high)(rng);
    }

    int rollInt(IntRange r, std::mt19937& rng) {
        return std::uniform_int_distribution<int>(r.low, r.high)(rng);
    }
}

std::mt19937& globalRng() {
    static std::mt19937 rng{ std::random_device{}() };
    return rng;
}

Genome rollGenome(const ShapeSpec& s, std::mt19937& rng) {
    std::uniform_int_distribution<int> ch(50, 255);
    auto col = [&] {
        return sf::Color(static_cast<std::uint8_t>(ch(rng)),
            static_cast<std::uint8_t>(ch(rng)),
            static_cast<std::uint8_t>(ch(rng)));
        };

    Genome g;
    g.spec = &s;
    g.startSize = rollFloat(s.startSize, rng);
    g.startThick = rollFloat(s.startThick, rng);
    g.numChildren = rollInt(s.numChildren, rng);
    g.maxDepth = rollInt(s.maxDepth, rng);
    g.childScale = rollFloat(s.childScale, rng);
    g.spreadFactor = rollFloat(s.spreadFactor, rng);
    g.growthInterval = rollFloat(s.growthInterval, rng);
    g.thickRatio = rollFloat(s.thickRatio, rng);
    g.innerColor = col();
    g.outerColor = col();
    return g;
}

Genome randomShapeGenome(std::mt19937& rng) {
    std::uniform_int_distribution<size_t> pick(0, std::size(kShapes) - 1);
    return rollGenome(kShapes[pick(rng)], rng);
}