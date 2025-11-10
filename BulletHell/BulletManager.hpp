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

    // spawn helpers
    void spawn(const sf::Vector2f& pos, const sf::Vector2f& vel);
    // collision check: circle radius
    bool checkCollision(const sf::Vector2f& point, float radius);

private:
    std::vector<Bullet> pool;
    sf::VertexArray verts; // draw points
};
