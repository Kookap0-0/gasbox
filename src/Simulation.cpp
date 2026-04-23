#include "Simulation.hpp"
#include <random>
#include <cmath>
#include <iostream>

Simulation::Simulation(unsigned int count)
    : particles(count),
      vertices(sf::Points),
      box(50.f, 750.f, 50.f, 550.f)
{
    // --- FPS ---
    if (!font.loadFromFile("fonts/Sekuya-Regular.ttf")) {
        std::cerr << "Failed to load font\n";
    }

    fpsText.setFont(font);
    fpsText.setCharacterSize(18);
    fpsText.setFillColor(sf::Color(220, 220, 220));
    fpsText.setPosition(10.f, 10.f);

    // --- Гистограмма ---
    histogramX.resize(HISTOGRAM_BINS);
    histogramY.resize(HISTOGRAM_BINS);

    for (int i = 0; i < HISTOGRAM_BINS; ++i) {
        histogramX[i] = static_cast<float>(i);
        histogramY[i] = 0.f;
    }

    speedDataSet = std::make_unique<PlotDataSet>(
        histogramX,
        histogramY,
        sf::Color(80, 220, 255),   // мягкий голубой
        "Speed Distribution",
        PlottingType::BARS
    );

    speedPlot = std::make_unique<SFPlot>(
        sf::Vector2f(800.f, 20.f),     // позиция
        sf::Vector2f(580.f, 560.f),    // размер
        40.f,                          // отступы
        font,
        "Speed",
        "Count"
    );

    speedPlot->AddDataSet(*speedDataSet);

    // фиксированный масштаб (не дергается)
    speedPlot->SetupAxes(
        0.f,
        static_cast<float>(HISTOGRAM_BINS),
        0.f,
        100.f,
        1.f,
        20.f,
        sf::Color(180, 180, 180)
    );

    speedPlot->GenerateVertices();

    // --- Панель под график ---
    plotPanel.setPosition(790.f, 10.f);
    plotPanel.setSize({600.f, 580.f});
    plotPanel.setFillColor(sf::Color(18, 18, 22));
    plotPanel.setOutlineThickness(2.f);
    plotPanel.setOutlineColor(sf::Color(60, 60, 70));

    // --- Сетка ---
    float boxWidth = box.getRight() - box.getLeft();
    float boxHeight = box.getBottom() - box.getTop();

    gridWidth = static_cast<int>(std::ceil(boxWidth / cellSize));
    gridHeight = static_cast<int>(std::ceil(boxHeight / cellSize));
    grid.resize(gridWidth * gridHeight);

    // --- Частицы ---
    std::random_device rd;
    std::mt19937 rng(rd());

    std::uniform_real_distribution<float> posX(box.getLeft() + 20.f, box.getRight() - 20.f);
    std::uniform_real_distribution<float> posY(box.getTop() + 20.f, box.getBottom() - 20.f);
    std::uniform_real_distribution<float> vel(-50.f, 50.f);

    for (size_t i = 0; i < count; ++i) {
        particles.posX[i] = posX(rng);
        particles.posY[i] = posY(rng);
        particles.velX[i] = vel(rng);
        particles.velY[i] = vel(rng);
    }

    vertices.resize(count);
}

void Simulation::buildGrid()
{
    // Очищаем все ячейки
    for (auto& cell : grid) cell.clear();

    float left = box.getLeft();
    float top = box.getTop();

    for (size_t i = 0; i < particles.size(); ++i)
    {
        int cx = static_cast<int>((particles.posX[i] - left) / cellSize);
        int cy = static_cast<int>((particles.posY[i] - top) / cellSize);
        // Проверяем, что индексы в пределах сетки
        if (cx >= 0 && cx < gridWidth && cy >= 0 && cy < gridHeight)
        {
            int index = cx + cy * gridWidth;
            grid[index].push_back(static_cast<int>(i));
        }
    }
}

void Simulation::resolveCollisions()
{
    for (int iter = 0; iter < solverIterations; ++iter)
    {
        for (int cy = 0; cy < gridHeight; ++cy)
        {
            for (int cx = 0; cx < gridWidth; ++cx)
            {
                int key = cx + cy * gridWidth;

                // 1. Столкновения внутри текущей ячейки (попарно, i < j)
                const auto& cell = grid[key];
                for (size_t a = 0; a < cell.size(); ++a)
                    for (size_t b = a + 1; b < cell.size(); ++b)
                        resolveCollision(cell[a], cell[b]);

                // 2. Столкновения с соседними ячейками (только правые и нижние, чтобы не дублировать)
                for (int dy = -1; dy <= 1; ++dy)
                {
                    for (int dx = -1; dx <= 1; ++dx)
                    {
                        // Пропускаем текущую ячейку (уже обработана)
                        if (dx == 0 && dy == 0) continue;
                        // Обрабатываем только половину направлений: вправо, вниз, вниз-вправо, вниз-влево
                        if (dx < 0 || (dx == 0 && dy < 0)) continue;

                        int nx = cx + dx;
                        int ny = cy + dy;
                        if (nx >= 0 && nx < gridWidth && ny >= 0 && ny < gridHeight)
                        {
                            int neighborKey = nx + ny * gridWidth;
                            const auto& neighbor = grid[neighborKey];
                            for (int i : cell)
                                for (int j : neighbor)
                                    resolveCollision(i, j);
                        }
                    }
                }
            }
        }
    }
}

void Simulation::resolveCollision(size_t i, size_t j)
{
    float dx = particles.posX[j] - particles.posX[i];
    float dy = particles.posY[j] - particles.posY[i];
    float distSq = dx * dx + dy * dy;
    float minDist = particles.radius[i] + particles.radius[j];
    if (distSq >= minDist * minDist) return;

    float dist = std::sqrt(distSq);
    if (dist == 0.f) return;

    float invDist = 1.f / dist;
    float nx = dx * invDist;
    float ny = dy * invDist;

    // Позиционная коррекция
    float overlap = minDist - dist;
    float correction = overlap * 0.5f;
    particles.posX[i] -= correction * nx;
    particles.posY[i] -= correction * ny;
    particles.posX[j] += correction * nx;
    particles.posY[j] += correction * ny;

    // Коррекция скорости
    float dvx = particles.velX[i] - particles.velX[j];
    float dvy = particles.velY[i] - particles.velY[j];
    float dot = dvx * nx + dvy * ny;

    float invMass_i = particles.invMass[i];
    float invMass_j = particles.invMass[j];
    float impulse = (2.f * dot) / (invMass_i + invMass_j);

    particles.velX[i] -= impulse * invMass_i * nx;
    particles.velY[i] -= impulse * invMass_i * ny;
    particles.velX[j] += impulse * invMass_j * nx;
    particles.velY[j] += impulse * invMass_j * ny;
}

void Simulation::update(float dt)
{
    float subDt = dt / substeps;

    for (int step = 0; step < substeps; ++step)
    {
        // 1. Интеграция движения
        for (size_t i = 0; i < particles.size(); ++i)
        {
            particles.posX[i] += particles.velX[i] * subDt;
            particles.posY[i] += particles.velY[i] * subDt;
        }

        // 2. Построение пространственной сетки
        buildGrid();

        // 3. Разрешение столкновений между частицами
        resolveCollisions();

        // 4. Столкновения со стенами
        for (size_t i = 0; i < particles.size(); ++i)
        {
            box.handleCollision(particles.posX[i], particles.posY[i],
                                particles.velX[i], particles.velY[i],
                                particles.radius[i]);
        }
    }

    // 5. Обновление вершин для отрисовки
    for (size_t i = 0; i < particles.size(); ++i)
    {
        vertices[i].position = { particles.posX[i], particles.posY[i] };
        vertices[i].color = sf::Color::White;
    }
}

void Simulation::render(sf::RenderWindow& window)
{
    // частицы
    window.draw(vertices);
    box.render(window);

    // панель графика (фон)
    window.draw(plotPanel);

    // сам график
    if (speedPlot)
        window.draw(*speedPlot);

    // FPS поверх всего
    window.draw(fpsText);
}

void Simulation::updateFPS(float dt) {
    fpsUpdateTimer += dt;
    frameCount++;
    if (fpsUpdateTimer >= 1.f) {
        currentFPS = frameCount;
        fpsText.setString("FPS: " + std::to_string(currentFPS));
        frameCount = 0;
        fpsUpdateTimer = 0.f;
    }
}


void Simulation::updateHistogram(float dt)
{
    histogramUpdateTimer += dt;
    if (histogramUpdateTimer < 0.3f) return;
    histogramUpdateTimer = 0.f;

    std::fill(histogramY.begin(), histogramY.end(), 0.f);

    float binWidth = MAX_SPEED / HISTOGRAM_BINS;

    for (size_t i = 0; i < particles.size(); ++i) {
        float speed = std::sqrt(
            particles.velX[i] * particles.velX[i] +
            particles.velY[i] * particles.velY[i]
        );

        int bin = static_cast<int>(speed / binWidth);
        if (bin < 0) bin = 0;
        if (bin >= HISTOGRAM_BINS) bin = HISTOGRAM_BINS - 1;

        histogramY[bin]++;
    }

    // --- вычисляем максимум ---
    float maxY = 1.f;
    for (float v : histogramY)
        if (v > maxY) maxY = v;

    maxY *= 1.2f;

    // ❗ КЛЮЧ: пересоздаём dataset
    speedDataSet = std::make_unique<PlotDataSet>(
        histogramX,
        histogramY,
        sf::Color(80, 220, 255, 200),
        "",
        PlottingType::BARS
    );

    // ❗ очищаем график полностью
    speedPlot->ClearVertices();

    // ❗ (если есть метод — лучше вызвать)
    // speedPlot->RemoveAllDataSets();

    // ❗ добавляем заново
    speedPlot->AddDataSet(*speedDataSet);

    // обновляем оси
    speedPlot->SetupAxes(
        0.f,
        static_cast<float>(HISTOGRAM_BINS),
        0.f,
        maxY,
        1.f,
        maxY / 5.f,
        sf::Color(180, 180, 180)
    );

    speedPlot->GenerateVertices();
}