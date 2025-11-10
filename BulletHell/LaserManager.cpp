#include "pch.h"
#include "LaserManager.hpp"
#include <algorithm>
#include <cmath>

LaserManager::LaserManager(size_t poolSize) {
    pool.resize(poolSize);
    for (auto& l : pool) {
        l.rect.setSize({ 5.f, 30.f });
        l.rect.setFillColor(sf::Color::Cyan);
        l.active = false;
    }
}

void LaserManager::spawn(const sf::Vector2f& pos) {
    for (auto& l : pool) {
        if (!l.active) {
            l.active = true;
            // SFML 3: setPosition accepts a vector; use sf::Vector2f
            l.rect.setPosition(sf::Vector2f(pos.x - 2.5f, pos.y - 30.f));
            return;
        }
    }
}

void LaserManager::update(float dt) {
    for (auto& l : pool) if (l.active) l.update(dt);
}

void LaserManager::render(sf::RenderTarget& rt) {
    for (auto& l : pool) if (l.active) rt.draw(l.rect);
}

bool LaserManager::checkCollision(sf::RectangleShape& target) {
    bool hit = false;
    for (auto& l : pool) {
        if (!l.active) continue;

        // Use .position and .size instead of .left/.width for SFML 3
        auto a = l.rect.getGlobalBounds();
        auto b = target.getGlobalBounds();

        float aLeft   = a.position.x;
        float aTop    = a.position.y;
        float aRight  = a.position.x + a.size.x;
        float aBottom = a.position.y + a.size.y;

        float bLeft   = b.position.x;
        float bTop    = b.position.y;
        float bRight  = b.position.x + b.size.x;
        float bBottom = b.position.y + b.size.y;

        bool overlap =
            !(aRight  < bLeft ||
              bRight  < aLeft ||
              aBottom < bTop  ||
              bBottom < aTop);

        if (overlap) {
            l.active = false;
            hit = true;
        }
    }
    return hit;
}

// Rectangle-vs-circle collision. Use the shape transform to compute the circle center
// so this works whether the CircleShape origin is (0,0) or centered.
bool LaserManager::checkCollision(const sf::CircleShape& shape) {
    bool hit = false;

    // getRadius() is local radius; transformPoint maps local center to world coords correctly
    float radius = shape.getRadius();
    sf::Vector2f center = shape.getTransform().transformPoint({ radius, radius });
    float radiusSq = radius * radius;

    for (auto& l : pool) {
        if (!l.active) continue;

        auto rBounds = l.rect.getGlobalBounds();
        float left   = rBounds.position.x;
        float top    = rBounds.position.y;
        float right  = rBounds.position.x + rBounds.size.x;
        float bottom = rBounds.position.y + rBounds.size.y;

        // Find closest point on rectangle to circle center
        float closestX = std::max(left, std::min(center.x, right));
        float closestY = std::max(top,  std::min(center.y, bottom));

        float dx = center.x - closestX;
        float dy = center.y - closestY;

        if (dx * dx + dy * dy <= radiusSq) {
            l.active = false;
            hit = true;
        }
    }

    return hit;
}