#include "Plot.hpp"
#include <random>
#include <cmath>
#include "Simulation.hpp"
#include <algorithm>

Plot::Plot(const Simulation& s) : sim(s) {
    histX.resize(HIST_BINS);
    histY.resize(HIST_BINS, 0.f);
    histTotal.resize(HIST_BINS, 0.f);
    theoryX.resize(THEORY_POINTS);
    theoryY.resize(THEORY_POINTS);
    rebuildHistogramAxes();
}

void Plot::updateHistogram(float dt, const ParticleSoA& particles, float temperature) {
    histUpdateTimer += dt;
    if (histUpdateTimer < 0.5f) return;
    histUpdateTimer = 0.f;

    std::fill(histY.begin(), histY.end(), 0.f);

    float binWidth = maxSpeed / HIST_BINS;

    // current histogram
    for (size_t i = 0; i < particles.size(); ++i) {
        float speed = std::hypot(particles.velX[i], particles.velY[i]);

        int bin = static_cast<int>(speed / binWidth);
        if (bin < 0) bin = 0;
        if (bin >= HIST_BINS) bin = HIST_BINS - 1;

        histY[bin]++;
    }

    // accumulate over time
    for (int i = 0; i < HIST_BINS; ++i) {
        histTotal[i] += histY[i];
    }

    ++histSamples; // ВАЖНО: без этого второй график будет "расти"


    float kT = temperature;
    if (kT < 0.01f) kT = 0.01f;

    for (int i = 0; i < THEORY_POINTS; ++i) {
        float v = theoryX[i];
        float pdf = (1.f / kT) * v * std::exp(-v * v / (2.f * kT));
        theoryY[i] = pdf;
    }
}

void Plot::rebuildHistogramAxes() {
    float binWidth = maxSpeed / HIST_BINS;

    for (int i = 0; i < HIST_BINS; ++i) {
        histX[i] = (i + 0.5f) * binWidth;
    }

    for (int i = 0; i < THEORY_POINTS; ++i) {
        theoryX[i] = maxSpeed * float(i) / float(THEORY_POINTS - 1);
    }
}

void Plot::setPlotRange(float newMaxSpeed)
{
    maxSpeed = std::max(newMaxSpeed, 1.f);
    rebuildHistogramAxes();
}

void Plot::resetHistogramStatistics()
{
    std::fill(histY.begin(), histY.end(), 0.f);
    std::fill(histTotal.begin(), histTotal.end(), 0.f);
    histSamples = 0;
    histUpdateTimer = 0.5f;
}