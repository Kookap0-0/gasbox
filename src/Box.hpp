#pragma once

class Box {
private:
    float left;
    float right;
    float top;
    float bottom;

public:
    Box(float x, float y, float width, float height);

    float getLeft() const;
    float getRight() const;
    float getTop() const;
    float getBottom() const;
};