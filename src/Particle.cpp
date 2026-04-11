#include "Particle.hpp"

Particle::Particle(float x, float y, float vx, float vy, float r, float m)
    : x(x), y(y), vx(vx), vy(vy), radius(r), mass(m), shape(r)
{
    shape.setFillColor(sf::Color::White);
    shape.setOrigin(r, r); // центрируем
    shape.setPosition(x, y);
}

void Particle::update(float dt) {
    x += vx * dt;
    y += vy * dt;
    shape.setPosition(x, y);
}

void Particle::checkWallCollision(float left, float right, float top, float bottom) {

    // Левая стенка
    if (x - radius <= left) {
        x = left + radius;
        vx = -vx;
    }

    // Правая стенка
    if (x + radius >= right) {
        x = right - radius;
        vx = -vx;
    }

    // Верхняя стенка
    if (y - radius <= top) {
        y = top + radius;
        vy = -vy;
    }

    // Нижняя стенка
    if (y + radius >= bottom) {
        y = bottom - radius;
        vy = -vy;
    }
}

float Particle::getX() const { return x; }
float Particle::getY() const { return y; }
float Particle::getRadius() const { return radius; }
const sf::CircleShape& Particle::getShape() const
{
    return shape;
}