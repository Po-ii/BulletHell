#pragma once
#include <SFML/Graphics.hpp>
#include "BulletManager.hpp"

class Enemy {
public:
    Enemy(const sf::Vector2f& p);
    void update(float dt);
    void draw(sf::RenderTarget& rt) const;

    // pattern spawners
    void spawnCircular(BulletManager& mgr, int count, float speed);
    void spawnSpiral(BulletManager& mgr, int count, float speed, float rotDelta);

private:
    sf::Vector2f pos;
    sf::CircleShape shape;
    float spiralAngle;
};