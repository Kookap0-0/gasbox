#include "Simulation.hpp"
#include <random>
#include <iostream>

Simulation::Simulation(unsigned int count)
    : vertices(sf::Points),
      box(50.f, 750.f, 50.f, 550.f)
{
    particles.reserve(count);

    for (unsigned int i = 0; i < count; ++i)
    {
        float x = 100.f + static_cast<float>(rand()) / RAND_MAX * 600.f;
        float y = 100.f + static_cast<float>(rand()) / RAND_MAX * 400.f;
        float vx = -50.f + static_cast<float>(rand()) / RAND_MAX * 100.f;
        float vy = -50.f + static_cast<float>(rand()) / RAND_MAX * 100.f;

        particles.emplace_back(x, y, vx, vy, 1.f, 1.f);
    }

    vertices.resize(count);
}

void Simulation::update(float dt)
{
    float subDt = dt / static_cast<float>(substeps);

    for (int step = 0; step < substeps; ++step)
    {
        // 1. Двигаем
        for (auto& p : particles)
            p.update(subDt);

        // 2. Строим grid
        grid.clear();

        for (size_t i = 0; i < particles.size(); ++i)
        {
            int cellX = static_cast<int>(particles[i].getX() / cellSize);
            int cellY = static_cast<int>(particles[i].getY() / cellSize);
            int key = getCellKey(cellX, cellY);
            grid[key].push_back(i);
        }

        // 3. Столкновения
        for (int iter = 0; iter < solverIterations; ++iter)
        {
            for (auto& [key, cellParticles] : grid)
            {
                int cellX = key % 10000;
                int cellY = key / 10000;

                for (int offsetX = -1; offsetX <= 1; ++offsetX)
                {
                    for (int offsetY = -1; offsetY <= 1; ++offsetY)
                    {
                        int neighborKey = getCellKey(cellX + offsetX,
                                                     cellY + offsetY);

                        if (grid.find(neighborKey) == grid.end())
                            continue;

                        auto& neighborParticles = grid[neighborKey];

                        for (int i : cellParticles)
                        {
                            for (int j : neighborParticles)
                            {
                                if (i < j)
                                    particles[i].resolveCollision(particles[j]);
                            }
                        }
                    }
                }
            }
        }

        // 4. Стены
        for (auto& p : particles)
            box.handleCollision(p);
    }

    // 5. Обновляем vertices ОДИН РАЗ
    for (size_t i = 0; i < particles.size(); ++i)
    {
        vertices[i].position = {
            particles[i].getX(),
            particles[i].getY()
        };

        vertices[i].color = sf::Color::White;
    }
}

void Simulation::render(sf::RenderWindow& window)
{
    window.draw(vertices);
    box.render(window);
}

int Simulation::getCellKey(int cellX, int cellY) const
{
    return cellX + cellY * 10000;
}