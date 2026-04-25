#include "Simulation.hpp"
#include <random>
#include <cmath>
#include <algorithm>

Simulation::Simulation(unsigned int count)
    : particles(count),
      vertices(sf::PrimitiveType::Points),
      box(50.f, 750.f, 50.f, 550.f)
{
    // grid
    float boxWidth = box.getRight() - box.getLeft();
    float boxHeight = box.getBottom() - box.getTop();

    gridWidth = static_cast<int>(std::ceil(boxWidth / cellSize));
    gridHeight = static_cast<int>(std::ceil(boxHeight / cellSize));
    grid.resize(gridWidth * gridHeight);

    // particles
    std::random_device rd;
    std::mt19937 rng(rd());

    std::uniform_real_distribution<float> posX(box.getLeft() + 20.f, box.getRight() - 20.f);
    std::uniform_real_distribution<float> posY(box.getTop() + 20.f, box.getBottom() - 20.f);
    std::uniform_real_distribution<float> vel(-50.f, 50.f);

    for (size_t i = 0; i < count; ++i) {
        particles.posX[i] = posX(rng);
        particles.posY[i] = posY(rng);
        particles.velX[i] = vel(rng);
        particles.velY[i] = vel(rng);
    }

    vertices.resize(count);
}

void Simulation::buildGrid()
{
    for (auto& cell : grid) {
        cell.clear();
    }

    float left = box.getLeft();
    float top = box.getTop();

    for (size_t i = 0; i < particles.size(); ++i) {
        int cx = static_cast<int>((particles.posX[i] - left) / cellSize);
        int cy = static_cast<int>((particles.posY[i] - top) / cellSize);

        if (cx >= 0 && cx < gridWidth && cy >= 0 && cy < gridHeight) {
            int index = cx + cy * gridWidth;
            grid[index].push_back(static_cast<int>(i));
        }
    }
}

void Simulation::resolveCollisions()
{
    for (int iter = 0; iter < solverIterations; ++iter) {
        for (int cy = 0; cy < gridHeight; ++cy) {
            for (int cx = 0; cx < gridWidth; ++cx) {
                int key = cx + cy * gridWidth;
                const auto& cell = grid[key];

                // collisions inside cell
                for (size_t a = 0; a < cell.size(); ++a) {
                    for (size_t b = a + 1; b < cell.size(); ++b) {
                        resolveCollision(cell[a], cell[b]);
                    }
                }

                // neighboring cells, without duplicates
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        if (dx == 0 && dy == 0) continue;
                        if (dx < 0 || (dx == 0 && dy < 0)) continue;

                        int nx = cx + dx;
                        int ny = cy + dy;

                        if (nx >= 0 && nx < gridWidth && ny >= 0 && ny < gridHeight) {
                            int nkey = nx + ny * gridWidth;
                            const auto& neighbor = grid[nkey];

                            for (int i : cell) {
                                for (int j : neighbor) {
                                    resolveCollision(i, j);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void Simulation::resolveCollision(size_t i, size_t j)
{
    float dx = particles.posX[j] - particles.posX[i];
    float dy = particles.posY[j] - particles.posY[i];

    float distSq = dx * dx + dy * dy;
    float minDist = particles.radius[i] + particles.radius[j];

    if (distSq >= minDist * minDist) return;

    float dist = std::sqrt(distSq);
    if (dist == 0.f) return;

    float invDist = 1.f / dist;
    float nx = dx * invDist;
    float ny = dy * invDist;

    // positional correction
    float overlap = minDist - dist;
    float correction = overlap * 0.5f;

    particles.posX[i] -= correction * nx;
    particles.posY[i] -= correction * ny;
    particles.posX[j] += correction * nx;
    particles.posY[j] += correction * ny;

    // velocity correction
    float dvx = particles.velX[i] - particles.velX[j];
    float dvy = particles.velY[i] - particles.velY[j];
    float dot = dvx * nx + dvy * ny;

    float invMass_i = particles.invMass[i];
    float invMass_j = particles.invMass[j];

    float denom = invMass_i + invMass_j;
    if (denom <= 0.f) return;

    float impulse = (2.f * dot) / denom;

    particles.velX[i] -= impulse * invMass_i * nx;
    particles.velY[i] -= impulse * invMass_i * ny;
    particles.velX[j] += impulse * invMass_j * nx;
    particles.velY[j] += impulse * invMass_j * ny;
}

void Simulation::update(float dt)
{
    float subDt = dt / substeps;

    for (int step = 0; step < substeps; ++step) {
        for (size_t i = 0; i < particles.size(); ++i) {
            particles.posX[i] += particles.velX[i] * subDt;
            particles.posY[i] += particles.velY[i] * subDt;
        }

        buildGrid();
        resolveCollisions();

        for (size_t i = 0; i < particles.size(); ++i) {
            box.handleCollision(
                particles.posX[i], particles.posY[i],
                particles.velX[i], particles.velY[i],
                particles.radius[i]
            );
        }
    }

    for (size_t i = 0; i < particles.size(); ++i) {
        vertices[i].position = { particles.posX[i], particles.posY[i] };
        vertices[i].color = sf::Color::White;
    }
}

void Simulation::render(sf::RenderWindow& window)
{
    window.draw(vertices);
    box.render(window);
}


float Simulation::getCurrentTemperature() const
{
    float totalKE = 0.f;
    for (size_t i = 0; i < particles.size(); ++i) {
        float v2 = particles.velX[i] * particles.velX[i] +
                   particles.velY[i] * particles.velY[i];
        totalKE += 0.5f * v2;
    }

    if (particles.size() == 0) return 0.f;
    return totalKE / static_cast<float>(particles.size());
}

void Simulation::setTemperature(float targetT)
{
    if (targetT <= 0.f) return;

    float currentT = getCurrentTemperature();
    if (currentT <= 0.f) return;

    float scale = std::sqrt(targetT / currentT);

    for (size_t i = 0; i < particles.size(); ++i) {
        particles.velX[i] *= scale;
        particles.velY[i] *= scale;
    }
}



