#include "ParticleSoA.hpp"
#include <random>

ParticleSoA::ParticleSoA(size_t count) {
    posX.resize(count);
    posY.resize(count);
    velX.resize(count);
    velY.resize(count);
    radius.resize(count, 1.f);
    invMass.resize(count, 1.f);
}