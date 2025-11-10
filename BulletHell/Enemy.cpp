#include "pch.h"
#include "Enemy.hpp"
#include <cmath>

Enemy::Enemy(const sf::Vector2f& p)
    : pos(p), spiralAngle(0.f), shootTimer(0.f), patternType(0), shotsBeforePause(3)
{
    shape.setRadius(14.f);
    shape.setOrigin(sf::Vector2f(14.f, 14.f));
    shape.setPosition(pos);
    shape.setFillColor(sf::Color::Magenta);
    // interval initial aléatoire 0.5-1.5s
    shootInterval = 0.5f + (std::rand() % 100) / 100.f;
}

void Enemy::update(float dt, BulletManager& mgr) {
    shootTimer += dt;

    if (shootTimer >= shootInterval) {
        shootTimer = 0.f;

        // sélection du pattern actuel
        switch (patternType) {
        case 0: spawnCircular(mgr); break;
        case 1: spawnSpiral(mgr); break;
        case 2: spawnBurst(mgr); break;
        }

        // réduire compteur de tirs avant pause
        shotsBeforePause--;
        if (shotsBeforePause <= 0) {
            // pause plus longue 1-2s
            shootInterval = 1.f + (std::rand() % 100) / 100.f;
            shotsBeforePause = 3 + (std::rand() % 3); // 3-5 tirs avant pause
        }
        else {
            // interval normal aléatoire rapide
            shootInterval = 0.3f + (std::rand() % 100) / 100.f;
        }

        // choisir pattern aléatoire pour le prochain tir
        patternType = std::rand() % 3;
    }
}

void Enemy::draw(sf::RenderTarget& rt) const {
    rt.draw(shape);
}

void Enemy::spawnCircular(BulletManager& mgr) {
    int count = 20;
    for (int i = 0; i < count; i++) {
        float a = (i / float(count)) * 2.f * 3.1415f;
        sf::Vector2f v{ cos(a) * 120.f, sin(a) * 120.f };
        mgr.spawn(pos, v, 5.f, sf::Color::Yellow);
    }
}

void Enemy::spawnSpiral(BulletManager& mgr) {
    int count = 20;
    for (int i = 0; i < count; i++) {
        float a = spiralAngle + (i / float(count)) * 2.f * 3.1415f;
        sf::Vector2f v{ cos(a) * 100.f, sin(a) * 100.f };
        mgr.spawn(pos, v, 5.f, sf::Color(255, 165, 0));
    }
    spiralAngle += 0.1f;
}

void Enemy::spawnBurst(BulletManager& mgr) {
    int count = 12;
    for (int i = 0; i < count; i++) {
        float a = (std::rand() % 360) / 180.f * 3.1415f;
        float speed = 80.f + std::rand() % 80;
        sf::Vector2f v{ cos(a) * speed, sin(a) * speed };
        float r;
        sf::Color c;
        int t = std::rand() % 3;
        if (t == 0) { r = 3.f; c = sf::Color::Yellow; }
        else if (t == 1) { r = 5.f; c = sf::Color(255, 165, 0); }
        else { r = 8.f; c = sf::Color::Red; }
        mgr.spawn(pos, v, r, c);
    }
}