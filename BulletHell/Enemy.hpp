#pragma once
#include <SFML/Graphics.hpp>
#include "BulletManager.hpp"

class Enemy {
public:
    Enemy(const sf::Vector2f& p);
    void update(float dt, BulletManager& mgr);
    void draw(sf::RenderTarget& rt) const;

    // pattern spawners
    void spawnCircular(BulletManager& mgr);
    void spawnSpiral(BulletManager& mgr);
    void spawnBurst(BulletManager& mgr);

private:
    sf::Vector2f pos;
    sf::CircleShape shape;
    float spiralAngle;

    // tir timing
    float shootTimer;
    float shootInterval;
    int patternType; // 0=circular,1=spiral,2=burst
    int shotsBeforePause;
};
