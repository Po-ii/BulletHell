#pragma once
#include "Laser.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

class LaserManager {
public:
    LaserManager(size_t poolSize = 100);
    void update(float dt);
    void render(sf::RenderTarget& rt);
    void spawn(const sf::Vector2f& pos);

    bool checkCollision(sf::RectangleShape& target);

    // Add this overload to LaserManager to allow collision checks with sf::CircleShape
    bool checkCollision(const sf::CircleShape& shape);

private:
    std::vector<Laser> pool;
};