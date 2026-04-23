#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Box.hpp"
#include "ParticleSoA.hpp"
#include <memory>

class Simulation
{
private:
    ParticleSoA particles;
    sf::VertexArray vertices;
    Box box;

    // параметры сетки
    float cellSize = 4.f;
    int gridWidth = 0;
    int gridHeight = 0;
    std::vector<std::vector<int>> grid;

    int substeps = 4;
    int solverIterations = 2;

    // FPS
    //sf::Font font;
    //sf::Text fpsText;
    float fpsUpdateTimer = 0.f;
    int frameCount = 0;
    int currentFPS = 0;

    // вспомогательные методы
    void buildGrid();
    void resolveCollisions();

    std::vector<float> histY;  // значения гистограммы (количество частиц в каждом бине)
    static constexpr int HIST_BINS = 30;
    static constexpr float MAX_SPEED = 100.f;
    float histUpdateTimer = 0.f;

public:
    Simulation(unsigned int count);
    void update(float dt);
    void render(sf::RenderWindow& window);
    void resolveCollision(size_t i, size_t j);
    void updateFPS(float dt);
    void updateHistogram(float dt);
    const std::vector<float>& getHistogram() const { return histY; }
    int getHistogramBins() const { return HIST_BINS; }
    std::vector<float> getTheoreticalDistribution() const;
};