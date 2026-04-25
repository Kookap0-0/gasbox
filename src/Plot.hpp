#pragma once
#include <vector>
#include <cstddef>

class Simulation; // forward declaration
class ParticleSoA;

class Plot {
private:
    const Simulation& sim; // ссылка на симуляцию
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
    explicit Plot(const Simulation& sim); // конструктор

    void rebuildHistogramAxes();
    void setPlotRange(float newMaxSpeed);
    void resetHistogramStatistics();
    void updateHistogram(float dt, const ParticleSoA& particles, float temperature);

    const std::vector<float>& getHistogramCurrent() const { return histY; }
    const std::vector<float>& getHistogramTotal() const { return histTotal; }
    const std::vector<float>& getHistogramX() const { return histX; }
    const std::vector<float>& getTheoreticalX() const { return theoryX; }
    const std::vector<float>& getTheoreticalY() const { return theoryY; }

    float getBinWidth() const { return maxSpeed / HIST_BINS; }
    int getHistogramBins() const { return HIST_BINS; }
    std::size_t getHistogramSamples() const { return histSamples; }
};