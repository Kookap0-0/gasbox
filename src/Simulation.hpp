#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Box.hpp"
#include "ParticleSoA.hpp"

class Simulation
{
private:
    ParticleSoA particles;
    sf::VertexArray vertices;
    Box box;

    // spatial grid
    float cellSize = 4.f;
    int gridWidth = 0;
    int gridHeight = 0;
    std::vector<std::vector<int>> grid;

    int substeps = 4;
    int solverIterations = 2;

    void buildGrid();
    void resolveCollisions();

public:
    Simulation(unsigned int count);

    void update(float dt);
    void render(sf::RenderWindow& window);

    void resolveCollision(size_t i, size_t j);

    void updateFPS(float dt);
    float getCurrentTemperature() const;
    void setTemperature(float targetT);

    const ParticleSoA& getParticles() const { return particles; }

};