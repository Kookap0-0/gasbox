#pragma once

class Particle
{
private:
    float x, y;
    float vx, vy;
    float radius;
    float mass;

public:
    Particle(float x, float y, float vx, float vy, float r, float m);

    void update(float dt);
    void resolveCollision(Particle& other);

    void invertVx();
    void invertVy();
    float getRadius() const;
    float getMass() const;
    float getX() const;
    float getY() const;
    float getVX() const;
    float getVY() const;
    void setX(float newX);
    void setY(float newY);  
    
};