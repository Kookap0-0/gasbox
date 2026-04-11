#pragma once
#include <SFML/Graphics.hpp>

class Particle {
private:
    float x, y;
    float vx, vy;
    float radius;
    float mass;
    sf::CircleShape shape;

public:
    Particle(float x, float y, float vx, float vy, float r, float m);

    void update(float dt);
    void checkWallCollision(float left, float right, float top, float bottom);

    float getX() const;
    float getY() const;
    float getRadius() const;
    const sf::CircleShape& getShape() const;
};