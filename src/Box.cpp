#include "Box.hpp"

Box::Box(float x, float y, float width, float height)
{
    left   = x;
    right  = x + width;
    top    = y;
    bottom = y + height;
}

float Box::getLeft() const   { return left; }
float Box::getRight() const  { return right; }
float Box::getTop() const    { return top; }
float Box::getBottom() const { return bottom; }