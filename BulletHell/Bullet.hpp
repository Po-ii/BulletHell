#pragma once
#include <SFML/Graphics.hpp>

struct Bullet {
    sf::Vector2f pos;
    sf::Vector2f vel;
    bool active = false;
    float size = 2.f;

    void update(float dt) {
        pos += vel * dt;
    }
};