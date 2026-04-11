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
    // 1️⃣ Двигаем частицы
    for (size_t i = 0; i < particles.size(); ++i)
    {
        particles[i].update(dt);
    }

    // 2️⃣ Перестраиваем spatial grid
    grid.clear();
    grid.reserve(particles.size());

    for (size_t i = 0; i < particles.size(); ++i)
    {
        int cellX = static_cast<int>(particles[i].getX() / cellSize);
        int cellY = static_cast<int>(particles[i].getY() / cellSize);

        int key = getCellKey(cellX, cellY);
        grid[key].push_back(i);
    }

    // 3️⃣ Итерационный solver столкновений
    const int solverIterations = 3;
    

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

                    auto it = grid.find(neighborKey);
                    if (it == grid.end())
                        continue;

                    auto& neighborParticles = it->second;

                    for (int i : cellParticles)
                    {
                        for (int j : neighborParticles)
                        {
                            if (i < j)
                            {
                                particles[i].resolveCollision(particles[j]);
                            }
                        }
                    }
                }
            }
        }

        // 💡 ВАЖНО: после каждой итерации снова проверяем стены
        for (size_t i = 0; i < particles.size(); ++i)
        {
            box.handleCollision(particles[i]);
        }
    }

    // 4️⃣ Обновляем рендер-данные
    for (size_t i = 0; i < particles.size(); ++i)
    {
        vertices[i].position = {
            particles[i].getX(),
            particles[i].getY()
        };

        vertices[i].color = sf::Color::White;
    }
    static float energyTimer = 0.f;
    energyTimer += dt;

    if (energyTimer >= 1.f) // выводим раз в секунду
    {
        energyTimer = 0.f;

        float totalEnergy = 0.f;

        for (auto& p : particles)
        {
            float vx = p.getVX();
            float vy = p.getVY();
            totalEnergy += 0.5f * (vx * vx + vy * vy);
        }

        std::cout << "Total Energy: " << totalEnergy << std::endl;
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