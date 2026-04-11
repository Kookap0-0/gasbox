#include "Particle.hpp"
#include <cmath>

Particle::Particle(float x, float y, float vx, float vy, float r, float m)
    : x(x), y(y), vx(vx), vy(vy), radius(r), mass(m) {}

void Particle::update(float dt) {
    x += vx * dt;
    y += vy * dt;
}

void Particle::invertVx()
{
    vx = -vx;
}

void Particle::invertVy()
{
    vy = -vy;
}


void Particle::resolveCollision(Particle& other)
{
    float dx = other.x - x;
    float dy = other.y - y;

    float distanceSquared = dx * dx + dy * dy;
    float minDistance = radius + other.radius;

    if (distanceSquared > minDistance * minDistance)
        return;

    float distance = std::sqrt(distanceSquared);

    if (distance == 0.f)
        return;

    float nx = dx / distance;
    float ny = dy / distance;

    // --- Позиционная коррекция ВСЕГДА ---
    float overlap = minDistance - distance;
    float correction = overlap * 0.5f;

    x -= correction * nx;
    y -= correction * ny;

    other.x += correction * nx;
    other.y += correction * ny;

    // --- Теперь считаем скорость ---
    float dvx = vx - other.vx;
    float dvy = vy - other.vy;

    float dot = dvx * nx + dvy * ny;

    if (dot > 0)
        return;

    float m1 = mass;
    float m2 = other.mass;

    float impulse = (2.f * dot) / (m1 + m2);

    vx -= impulse * m2 * nx;
    vy -= impulse * m2 * ny;

    other.vx += impulse * m1 * nx;
    other.vy += impulse * m1 * ny;
}

void Particle::setX(float newX)
{
    x = newX;
}

void Particle::setY(float newY)
{
    y = newY;
}

float Particle::getX() const { return x; }
float Particle::getY() const { return y; }
float Particle::getRadius() const { return radius; }
float Particle::getMass() const { return mass; }
float Particle::getVX() const { return vx; };
float Particle::getVY() const { return vy; };