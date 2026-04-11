#include "Box.hpp"

Box::Box(float l, float r, float t, float b)
    : left(l), right(r), top(t), bottom(b)
{
    border.setPosition(left, top);
    border.setSize({right - left, bottom - top});

    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::White);
    border.setOutlineThickness(1.f);
}

void Box::handleCollision(Particle& p) const
{
    float r = p.getRadius();

    // Левая стена
    if (p.getX() - r < left)
    {
        p.setX(left + r);
        p.invertVx();
    }

    // Правая стена
    if (p.getX() + r > right)
    {
        p.setX(right - r);
        p.invertVx();
    }

    // Верхняя стена
    if (p.getY() - r < top)
    {
        p.setY(top + r);
        p.invertVy();
    }

    // Нижняя стена
    if (p.getY() + r > bottom)
    {
        p.setY(bottom - r);
        p.invertVy();
    }
}

void Box::render(sf::RenderWindow& window) const
{
    window.draw(border);
}

float Box::getLeft() const { return left; }
float Box::getRight() const { return right; }
float Box::getTop() const { return top; }
float Box::getBottom() const { return bottom; }