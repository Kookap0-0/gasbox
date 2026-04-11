#include <SFML/Graphics.hpp>
#include "Simulation.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Gasbox");
    window.setFramerateLimit(60);

    Simulation sim(100);

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();

        sim.update(dt);

        window.clear(sf::Color::Black);
        sim.render(window);
        window.display();
    }

    return 0;
}