#pragma once
#include <SFML/Graphics.hpp>

struct Bullet {
    sf::Vector2f pos;
    sf::Vector2f vel;
    bool active = false;
    float radius = 4.f;
    sf::Color color = sf::Color::Red;

    void update(float dt) {
        pos += vel * dt;
    }
};