#pragma once

#include <vector>
#include "Particle.hpp"
#include "Box.hpp"
#include <SFML/Graphics.hpp>

class Simulation {
private:
    std::vector<Particle> particles;
    Box box;

public:
    Simulation(int particleCount);

    void update(float dt);
    void render(sf::RenderWindow& window);
};