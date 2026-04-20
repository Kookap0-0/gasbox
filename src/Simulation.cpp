#include "Simulation.hpp"
#include <random>
#include <cmath>
#include <iostream>

Simulation::Simulation(unsigned int count)
    : particles(count),
      vertices(sf::Points),
      box(50.f, 750.f, 50.f, 550.f)
{

    if (!font.loadFromFile("fonts/Sekuya-Regular.ttf")) {
    std::cerr << "Failed to load font arial.ttf" << std::endl;
    }
    fpsText.setFont(font);
    fpsText.setCharacterSize(20);
    fpsText.setFillColor(sf::Color::White);
    fpsText.setPosition(10.f, 10.f);
    fpsText.setString("FPS: 0");


    // Вычисляем размеры сетки на основе размеров коробки и cellSize
    float boxWidth = box.getRight() - box.getLeft();   // 700
    float boxHeight = box.getBottom() - box.getTop();  // 500
    gridWidth = static_cast<int>(std::ceil(boxWidth / cellSize));
    gridHeight = static_cast<int>(std::ceil(boxHeight / cellSize));
    
    // Инициализируем плоскую сетку: gridWidth * gridHeight пустых векторов
    grid.resize(gridWidth * gridHeight);

    // Генерация частиц
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
    window.draw(vertices);
    box.render(window);
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