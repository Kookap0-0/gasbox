#include "Box.hpp"

Box::Box(float l, float r, float t, float b)
    : left(l), right(r), top(t), bottom(b)
{
    border.setPosition({left, top});
    border.setSize({right - left, bottom - top});

    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::White);
    border.setOutlineThickness(1.f);
}

void Box::handleCollision(float& x, float& y, float& vx, float& vy, float radius) const {
    if (x - radius < left) {
        x = left + radius;
        vx = -vx;
    }
    if (x + radius > right) {
        x = right - radius;
        vx = -vx;
    }
    if (y - radius < top) {
        y = top + radius;
        vy = -vy;
    }
    if (y + radius > bottom) {
        y = bottom - radius;
        vy = -vy;
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