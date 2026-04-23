#include <SFML/Graphics.hpp>
#include "Simulation.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include <algorithm>
#include <optional>

int main()
{
    sf::RenderWindow window(sf::VideoMode({1800, 1000}), "Gasbox");
    window.setVerticalSyncEnabled(true);

    if (!ImGui::SFML::Init(window))
        return -1;

    ImGui::StyleColorsDark();

    Simulation sim(5000);

    const float dt = 1.f / 120.f;
    const float maxFrameTime = 0.25f;
    const int maxSteps = 5;

    float warmupElapsed = 0.f;
    while (warmupElapsed < 1.5f) {
        sim.update(dt);
        warmupElapsed += dt;
    }

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
        if (frameTime > maxFrameTime)
            frameTime = maxFrameTime;

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

        sim.updateHistogram(frameTime);
        ImGui::SFML::Update(window, sf::seconds(frameTime));

        const auto& hist = sim.getHistogram();
        const auto& theory = sim.getTheoreticalDistribution();

        ImGui::SetNextWindowSize(ImVec2(800.f, 650.f), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(1000.f, 20.f), ImGuiCond_FirstUseEver);

        ImGui::Begin("Speed Distribution");

        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Separator();

        if (!hist.empty() && !theory.empty())
        {
            float maxHist = *std::max_element(hist.begin(), hist.end());
            float maxTheory = *std::max_element(theory.begin(), theory.end());
            float yMax = std::max(maxHist, maxTheory) * 1.1f;
            if (yMax < 1.f) yMax = 1.f;

            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.2f, 0.5f, 1.0f, 0.8f));
            ImGui::PlotHistogram(
                "##hist",
                hist.data(),
                static_cast<int>(hist.size()),
                0,
                "Experimental distribution",
                0.f,
                yMax,
                ImVec2(750.f, 350.f),
                sizeof(float)
            );
            ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
            ImGui::PlotLines(
                "##theory",
                theory.data(),
                static_cast<int>(theory.size()),
                0,
                "Maxwell-Boltzmann",
                0.f,
                yMax,
                ImVec2(750.f, 350.f),
                sizeof(float)
            );
            ImGui::PopStyleColor();

            ImGui::Text("X: speed   Y: number of particles");
        }
        else
        {
            ImGui::Text("Waiting for data...");
        }

        ImGui::End();

        window.clear();
        sim.render(window);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}