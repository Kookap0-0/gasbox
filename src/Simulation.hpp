#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Box.hpp"
#include "ParticleSoA.hpp"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

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
    static constexpr int HISTOGRAM_BINS = 30;       // количество столбцов
    static constexpr float MAX_SPEED = 100.f;       // максимальная скорость (можно подобрать)
    std::vector<int> speedCounts;                   // количество частиц в каждом бине
    sf::RectangleShape histogramBars[HISTOGRAM_BINS]; // столбцы гистограммы
    sf::Text histogramTitle;                        // заголовок
    float histogramUpdateTimer = 0.f; 

    // Вспомогательные методы
    void buildGrid();
    void resolveCollisions();

public:
    Simulation(unsigned int count);
    void update(float dt);
    void render(sf::RenderWindow& window);
    void resolveCollision(size_t i, size_t j);
    void updateFPS(float dt);
};