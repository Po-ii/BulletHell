#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Bullet.hpp"

class BulletManager {
public:
    BulletManager(size_t poolSize = 2000);
    ~BulletManager();

    void update(float dt);
    void render(sf::RenderTarget& rt);
    void resetAll();

    void spawn(const sf::Vector2f& pos, const sf::Vector2f& vel, float radius = 4.f, sf::Color color = sf::Color::Red);
    bool checkCollision(const sf::Vector2f& point, float radius);

private:
    std::vector<Bullet> pool;
};