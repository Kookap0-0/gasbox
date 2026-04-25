#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Box.hpp"
#include "ParticleSoA.hpp"

class Simulation
{
private:
    ParticleSoA particles;
    sf::VertexArray vertices;
    Box box;

    // spatial grid
    float cellSize = 4.f;
    int gridWidth = 0;
    int gridHeight = 0;
    std::vector<std::vector<int>> grid;

    int substeps = 4;
    int solverIterations = 2;

    // FPS
    float fpsUpdateTimer = 0.f;
    int frameCount = 0;
    int currentFPS = 0;

    void buildGrid();
    void resolveCollisions();

    // histogram
    std::vector<float> histX;
    std::vector<float> histY;       // current frame
    std::vector<float> histTotal;   // accumulated over whole run

    std::vector<float> theoryX;
    std::vector<float> theoryY;

    std::size_t histSamples = 0;

    static constexpr int HIST_BINS = 50;
    static constexpr int THEORY_POINTS = 200;
    float maxSpeed = 120.f;

    float histUpdateTimer = 0.f;

public:
    Simulation(unsigned int count);

    void update(float dt);
    void render(sf::RenderWindow& window);

    void resolveCollision(size_t i, size_t j);

    void updateFPS(float dt);
    void updateHistogram(float dt);

    float getCurrentTemperature() const;
    void setTemperature(float targetT);

    void rebuildHistogramAxes();
    void setPlotRange(float newMaxSpeed);
    void resetHistogramStatistics();

    const std::vector<float>& getHistogramCurrent() const { return histY; }
    const std::vector<float>& getHistogramTotal() const { return histTotal; }
    const std::vector<float>& getHistogramX() const { return histX; }
    const std::vector<float>& getTheoreticalX() const { return theoryX; }
    const std::vector<float>& getTheoreticalY() const { return theoryY; }

    float getBinWidth() const { return maxSpeed / HIST_BINS; }
    int getHistogramBins() const { return HIST_BINS; }
    std::size_t getHistogramSamples() const { return histSamples; }
};