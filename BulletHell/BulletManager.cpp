#include "pch.h"
#include "BulletManager.hpp"
#include <cmath>

BulletManager::BulletManager(size_t poolSize)
    : pool(poolSize), verts(sf::PrimitiveType::Points)
{
    verts.resize(poolSize);
    // configure point size not directly supported; points will render 1px.
    // You can use quads/triangles for larger visuals (omitted for brevity).
}

BulletManager::~BulletManager() = default;

void BulletManager::resetAll() {
    for (auto& b : pool) b.active = false;
}

void BulletManager::spawn(const sf::Vector2f& pos, const sf::Vector2f& vel) {
    for (auto& b : pool) {
        if (!b.active) {
            b.active = true;
            b.pos = pos;
            b.vel = vel;
            return;
        }
    }
}

void BulletManager::update(float dt) {
    const sf::Vector2u winSize(960, 720);
    for (size_t i = 0; i < pool.size(); ++i) {
        Bullet& b = pool[i];
        if (!b.active) {
            verts[i].position = { -10000.f,-10000.f };
            continue;
        }
        b.update(dt);
        verts[i].position = b.pos;
        verts[i].color = sf::Color::Red;
        // deactivate if offscreen:
        if (b.pos.x < -50 || b.pos.x >(float)winSize.x + 50 || b.pos.y < -50 || b.pos.y >(float)winSize.y + 50)
            b.active = false;
    }
}

void BulletManager::render(sf::RenderTarget& rt) {
    rt.draw(verts);
}

bool BulletManager::checkCollision(const sf::Vector2f& point, float radius) {
    float r2 = radius * radius;
    for (auto& b : pool) {
        if (!b.active) continue;
        float dx = b.pos.x - point.x;
        float dy = b.pos.y - point.y;
        if (dx * dx + dy * dy < r2) return true;
    }
    return false;
}