#include "pch.h"
#include "Enemy.hpp"
#include <cmath>

Enemy::Enemy(const sf::Vector2f& p) : pos(p), spiralAngle(0.f) {
    shape.setRadius(14.f);
    shape.setOrigin(sf::Vector2f(14.f, 14.f)); // FIX: pass a Vector2f, not two floats
    shape.setPosition(pos);
    shape.setFillColor(sf::Color::Magenta);
}

void Enemy::update(float dt) {
    // could move or animate - keep static for demo
    (void)dt;
}

void Enemy::draw(sf::RenderTarget& rt) const {
    rt.draw(shape);
}

void Enemy::spawnCircular(BulletManager& mgr, int count, float speed) {
    for (int i = 0; i < count; ++i) {
        float a = (i / float(count)) * 2.f * 3.14159265359f;
        sf::Vector2f v{ std::cos(a) * speed, std::sin(a) * speed };
        mgr.spawn(pos, v);
    }
}

void Enemy::spawnSpiral(BulletManager& mgr, int count, float speed, float rotDelta) {
    for (int i = 0; i < count; ++i) {
        float a = spiralAngle + (i / float(count)) * 2.f * 3.14159265359f;
        sf::Vector2f v{ std::cos(a) * speed, std::sin(a) * speed };
        mgr.spawn(pos, v);
    }
    spiralAngle += rotDelta;
}
