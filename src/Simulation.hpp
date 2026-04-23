#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Box.hpp"
#include "ParticleSoA.hpp"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFGraphing/PlotDataSet.h>
#include <SFGraphing/SFPlot.h>
#include <memory>

using namespace csrc;

class Simulation
{
private:
    ParticleSoA particles;
    sf::VertexArray vertices;
    Box box;

    // Параметры сетки
    float cellSize = 4.f;               // фиксированный размер ячейки
    int gridWidth = 0;                  // будет вычислен в конструкторе
    int gridHeight = 0;                 // будет вычислен в конструкторе
    std::vector<std::vector<int>> grid; // плоский массив: grid[cellIndex]

    int substeps = 4;
    int solverIterations = 2;

    //Вывод FPS
    sf::Font font;
    sf::Text fpsText;
    float fpsUpdateTimer = 0.f;
    int frameCount = 0;
    int currentFPS = 0;

    // Гистограмма скоростей
    std::vector<float> histogramX;        // Значения по оси X (центры "корзин")
    std::vector<float> histogramY;        // Значения по оси Y (количество частиц)
    std::unique_ptr<PlotDataSet> speedDataSet;
    std::unique_ptr<SFPlot> speedPlot;                    // Объект самого графика
    float histogramUpdateTimer = 0.f;     // Таймер для обновления (как для FPS)
    static constexpr int HISTOGRAM_BINS = 10; // Количество столбцов
    static constexpr float MAX_SPEED = 100.f; // Максимальная скорость на графике
    static constexpr float FIXED_Y_MAX = 500.f; 


    // Вспомогательные методы
    void buildGrid();
    void resolveCollisions();

    sf::RectangleShape plotPanel;

public:
    Simulation(unsigned int count);
    void update(float dt);
    void render(sf::RenderWindow& window);
    void resolveCollision(size_t i, size_t j);
    void updateFPS(float dt);
    void updateHistogram(float dt);
};