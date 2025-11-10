#pragma once
#include <SFML/Graphics.hpp>

struct Laser {
    sf::RectangleShape rect;
    bool active = false;
    float speed = 500.f;

    void update(float dt) {
        rect.move(sf::Vector2f(0.f, -speed * dt));
        if (rect.getPosition().y < -rect.getSize().y) active = false;
    }
};