#include <SFML/Graphics.hpp>
#include "Simulation.hpp"
#include "Plot.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "implot.h"

#include <optional>
#include <algorithm>
#include <numeric>
#include <vector>
#include <cmath>

struct PlotScale {
    double xMax;
    double yMax;
};

static PlotScale MakeScale(float T)
{
    T = std::max(T, 0.0001f);

    // X ~ sqrt(T)
    double xMax = 6.0 * std::sqrt(T);

    // для 2D Maxwell пик ~ exp(-1/2)/sqrt(T)
    double yMax = 1.35 * std::exp(-0.5) / std::sqrt(T);

    return { xMax, yMax };
}


static std::vector<float> ToDensity(const std::vector<float>& counts, float totalCount, float binWidth)
{
    std::vector<float> out(counts.size(), 0.f);
    if (totalCount <= 0.f || binWidth <= 0.f) return out;

    for (size_t i = 0; i < counts.size(); ++i) {
        out[i] = counts[i] / (totalCount * binWidth);
    }
    return out;
}

static float MaxValue(const std::vector<float>& v)
{
    if (v.empty()) return 0.f;
    return *std::max_element(v.begin(), v.end());
}

int main()
{
    const unsigned int N = 5000;
    float temperatureInput = 1.0f;

    sf::RenderWindow window(sf::VideoMode({1800, 1000}), "Gasbox");
    window.setVerticalSyncEnabled(true);

    if (!ImGui::SFML::Init(window))
        return -1;

    ImPlot::CreateContext();
    ImGui::StyleColorsDark();

    Simulation sim(N);
    Plot plot(sim);
    
    

    const float dt = 1.f / 120.f;
    const float maxFrameTime = 0.25f;
    const int maxSteps = 5;

    // warmup
    {
        float warmup = 0.f;
        while (warmup < 1.5f) {
            sim.update(dt);
            warmup += dt;
        }
    }

    float currentTemperature = sim.getCurrentTemperature();
    temperatureInput = currentTemperature;
    PlotScale plotScale = MakeScale(currentTemperature);

    float accumulator = 0.f;
    sf::Clock clock;

    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float frameTime = clock.restart().asSeconds();
        frameTime = std::min(frameTime, maxFrameTime);

        accumulator += frameTime;

        int steps = 0;
        while (accumulator >= dt && steps < maxSteps)
        {
            sim.update(dt);
            accumulator -= dt;
            steps++;
        }

        if (steps == maxSteps)
            accumulator = 0.f;

        plot.updateHistogram(frameTime, sim.getParticles(), sim.getCurrentTemperature());
        ImGui::SFML::Update(window, sf::seconds(frameTime));

        const auto& x = plot.getHistogramX();
        const auto& current = plot.getHistogramCurrent();
        const auto& total = plot.getHistogramTotal();
        const auto& tx = plot.getTheoreticalX();
        const auto& ty = plot.getTheoreticalY();

        std::vector<float> currentDensity = ToDensity(current, static_cast<float>(N), plot.getBinWidth());
        std::vector<float> totalDensity = ToDensity(
            total,
            static_cast<float>(N) * static_cast<float>(std::max<std::size_t>(1, plot.getHistogramSamples())),
            plot.getBinWidth()
        );
        const std::vector<float>& theoryDensity = ty;

        ImGui::Begin("Controls");

        ImGui::InputFloat("Temperature", &temperatureInput, 0.1f, 1.0f, "%.3f");

        if (ImGui::Button("Apply temperature")) {
            sim.setTemperature(temperatureInput);

            float actualT = sim.getCurrentTemperature();
            temperatureInput = actualT;

            plotScale = MakeScale(actualT);
            plot.setPlotRange(static_cast<float>(plotScale.xMax));
            plot.resetHistogramStatistics();
        }


        ImGui::Separator();
        ImGui::Text("Current T: %.3f", sim.getCurrentTemperature());

        ImGui::End();

        ImGui::SetNextWindowSize(ImVec2(820.f, 920.f), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(960.f, 20.f), ImGuiCond_FirstUseEver);


        ImGui::Begin("Speed Distribution");

        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Separator();
        

        if (!currentDensity.empty() && !theoryDensity.empty())
        {
            // Масштаб для верхнего графика — по текущим данным
            float topMaxY = std::max(MaxValue(currentDensity), MaxValue(theoryDensity)) * 1.2f;
            float bottomMaxY = std::max(MaxValue(totalDensity), MaxValue(theoryDensity)) * 1.2f;

            if (topMaxY < 0.0001f) topMaxY = 0.0001f;
            if (bottomMaxY < 0.0001f) bottomMaxY = 0.0001f;

            if (ImPlot::BeginPlot("##CurrentPlot", ImVec2(780, 360)))
            {
                ImPlot::SetupAxes("Speed", "Probability");
                ImPlot::SetupAxisLimits(ImAxis_X1, 0.0, plotScale.xMax, ImGuiCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, plotScale.yMax, ImGuiCond_Always);

                ImPlotSpec barSpec;
                barSpec.LineColor = ImVec4(0.2f, 0.55f, 1.0f, 1.0f);
                barSpec.FillColor = ImVec4(0.2f, 0.55f, 1.0f, 0.35f);
                barSpec.FillAlpha = 0.35f;
                barSpec.LineWeight = 1.0f;

                ImPlot::PlotBars(
                    "Current",
                    x.data(),
                    currentDensity.data(),
                    static_cast<int>(x.size()),
                    plot.getBinWidth() * 0.9f,
                    barSpec
                );

                ImPlotSpec lineSpec;
                lineSpec.LineColor = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
                lineSpec.LineWeight = 2.5f;

                ImPlot::PlotLine(
                    "Maxwell-Boltzmann",
                    tx.data(),
                    theoryDensity.data(),
                    static_cast<int>(tx.size()),
                    lineSpec
                );

                ImPlot::EndPlot();
            }

            if (ImPlot::BeginPlot("##AccumulatedPlot", ImVec2(780, 360)))
            {
                ImPlot::SetupAxes("Speed", "Probability");
                ImPlot::SetupAxisLimits(ImAxis_X1, 0.0, plotScale.xMax, ImGuiCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, plotScale.yMax, ImGuiCond_Always);

                ImPlotSpec barSpec;
                barSpec.LineColor = ImVec4(0.2f, 1.0f, 0.4f, 1.0f);
                barSpec.FillColor = ImVec4(0.2f, 1.0f, 0.4f, 0.35f);
                barSpec.FillAlpha = 0.35f;
                barSpec.LineWeight = 1.0f;

                ImPlot::PlotBars(
                    "Accumulated",
                    x.data(),
                    totalDensity.data(),
                    static_cast<int>(x.size()),
                    plot.getBinWidth() * 0.9f,
                    barSpec
                );

                ImPlotSpec lineSpec;
                lineSpec.LineColor = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
                lineSpec.LineWeight = 2.5f;

                ImPlot::PlotLine(
                    "Maxwell-Boltzmann",
                    tx.data(),
                    theoryDensity.data(),
                    static_cast<int>(tx.size()),
                    lineSpec
                );

                ImPlot::EndPlot();
            }
        }
        else
        {
            ImGui::Text("Collecting data...");
        }

        ImGui::End();

        window.clear(sf::Color(18, 18, 22));
        sim.render(window);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImPlot::DestroyContext();
    ImGui::SFML::Shutdown();
    return 0;
}