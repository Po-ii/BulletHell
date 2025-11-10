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

    void takeDamage(int dmg) { hp -= dmg; if (hp < 0) hp = 0; }
    int getHP() const { return hp; }
    const sf::CircleShape& getShape() const { return shape; }

private:
    sf::Vector2f pos;

    // outer white sclera used for collisions
    sf::CircleShape shape;

    // extra shapes to render an "eye" appearance
    sf::CircleShape iris;
    sf::CircleShape pupil;
    sf::CircleShape highlight;

    float spiralAngle;

    int hp = 1000;

    // tir timing
    float shootTimer;
    float shootInterval;
    int patternType; // 0=circular,1=spiral,2=burst
    int shotsBeforePause;
};
