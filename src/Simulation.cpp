#include "Simulation.hpp"
#include <random>

Simulation::Simulation(int particleCount)
    : box(50.f, 50.f, 500.f, 500.f)
{
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> posX(60.f, 540.f);
    std::uniform_real_distribution<float> posY(60.f, 540.f);
    std::uniform_real_distribution<float> vel(-200.f, 200.f);

    for (int i = 0; i < particleCount; i++) {
        particles.emplace_back(
            posX(rng),
            posY(rng),
            vel(rng),
            vel(rng),
            1.f,
            1.f
        );
    }
}

void Simulation::update(float dt)
{
    for (auto& p : particles) {
        p.update(dt);
        p.checkWallCollision(
            box.getLeft(),
            box.getRight(),
            box.getTop(),
            box.getBottom()
        );
    }
}

void Simulation::render(sf::RenderWindow& window)
{
    // рисуем box
    sf::RectangleShape rect;
    rect.setPosition(box.getLeft(), box.getTop());
    rect.setSize(sf::Vector2f(
        box.getRight() - box.getLeft(),
        box.getBottom() - box.getTop()
    ));
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(2.f);

    window.draw(rect);

    // рисуем частицы
    for (const auto& p : particles)
    {
        window.draw(p.getShape());
    }
}