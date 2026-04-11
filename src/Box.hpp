#pragma once

#include "Particle.hpp"
#include <SFML/Graphics.hpp>

class Box
{
private:
    float left;
    float right;
    float top;
    float bottom;
    sf::RectangleShape border;

public:
    Box(float l, float r, float t, float b);

    void handleCollision(Particle& p) const;
    void render(sf::RenderWindow& window) const;
    float getLeft() const;
    float getRight() const;
    float getTop() const;
    float getBottom() const;
};