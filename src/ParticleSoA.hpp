#pragma once
#include <vector>

class ParticleSoA {
public:
    std::vector<float> posX, posY;     // позиции
    std::vector<float> velX, velY;     // скорости
    std::vector<float> radius;         // радиус (все 1.f)
    std::vector<float> invMass;        // 1/масса (все 1.f)

    ParticleSoA(size_t count);
    size_t size() const { return posX.size(); }
};