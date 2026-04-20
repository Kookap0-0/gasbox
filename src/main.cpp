#include <SFML/Graphics.hpp>
#include "Simulation.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1400, 1000), "Gasbox");
    window.setVerticalSyncEnabled(true); // вместо setFramerateLimit

    Simulation sim(1000);

    const float dt = 1.f / 120.f;     // фиксированный шаг физики
    const float maxFrameTime = 0.25f; // защита от скачков (250 мс)
    const int maxSteps = 5;           // максимум физических шагов за кадр


    const float warmupTime = 1.5f;  // 1.5 секунды прогрева
    float warmupElapsed = 0.f;
    while (warmupElapsed < warmupTime) {
        sim.update(dt);
        warmupElapsed += dt;
    }

    float accumulator = 0.f;
    sf::Clock clock;

    while (window.isOpen())
    {
        // 1️ Обработка событий
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // 2️ Замер времени
        float frameTime = clock.restart().asSeconds();

        if (frameTime > maxFrameTime)
            frameTime = maxFrameTime;

        accumulator += frameTime;

        // 3️ Фиксированная физика с защитой
        int steps = 0;
        while (accumulator >= dt && steps < maxSteps)
        {
            sim.update(dt);
            accumulator -= dt;
            steps++;
        }

        // если не успели догнать — сбрасываем хвост
        if (steps == maxSteps)
            accumulator = 0.f;

        // 4️ Отрисовка
        sim.updateFPS(frameTime);
        window.clear();
        sim.render(window);
        window.display();
    }

    return 0;
}