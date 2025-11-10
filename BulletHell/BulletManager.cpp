#include "pch.h"
#include "BulletManager.hpp"
#include <cmath>

BulletManager::BulletManager(size_t poolSize)
    : pool(poolSize)
{
}

BulletManager::~BulletManager() = default;

void BulletManager::resetAll() {
    for (auto& b : pool) b.active = false;
}

void BulletManager::spawn(const sf::Vector2f& pos, const sf::Vector2f& vel, float radius, sf::Color color) {
    for (auto& b : pool) {
        if (!b.active) {
            b.active = true;
            b.pos = pos;
            b.vel = vel;
            b.radius = radius;
            b.color = color;
            return;
        }
    }
}

void BulletManager::update(float dt) {
    const sf::Vector2u winSize(960, 720);
    for (auto& b : pool) {
        if (!b.active) continue;
        b.update(dt);
        if (b.pos.x < -100 || b.pos.x > winSize.x + 100 ||
            b.pos.y < -100 || b.pos.y > winSize.y + 100)
            b.active = false;
    }
}

void BulletManager::render(sf::RenderTarget& rt) {
    sf::CircleShape circle;
    for (auto& b : pool) {
        if (!b.active) continue;
        circle.setRadius(b.radius);
        circle.setOrigin(sf::Vector2f(b.radius, b.radius));
        circle.setPosition(b.pos);
        circle.setFillColor(b.color);
        rt.draw(circle);
    }
}

bool BulletManager::checkCollision(const sf::Vector2f& point, float radius) {
    for (auto& b : pool) {
        if (!b.active) continue;
        float dx = b.pos.x - point.x;
        float dy = b.pos.y - point.y;
        float totalRadius = radius + b.radius;               // consider both radii
        float r2 = totalRadius * totalRadius;
        if (dx * dx + dy * dy < r2) {
            b.active = false;                                // deactivate bullet on hit
            return true;
        }
    }
    return false;
}