#include <SFML/Graphics.hpp>
#include <vector>
#include "Particle.hpp"
#include "Box.hpp"
#include <unordered_map>


class Simulation
{
private:
    std::vector<Particle> particles;
    sf::VertexArray vertices;
    Box box;

    float cellSize = 4.f; // 2 * радиус (если радиус = 1)
    std::unordered_map<int, std::vector<int>> grid;

public:
    Simulation(unsigned int count);
    int getCellKey(int cellX, int cellY) const;
    void update(float dt);
    void render(sf::RenderWindow& window);
};