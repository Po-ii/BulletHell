#include "pch.h"
#include "Enemy.hpp"
#include <cmath>

Enemy::Enemy(const sf::Vector2f& p)
    : pos(p), spiralAngle(0.f), shootTimer(0.f), patternType(0), shotsBeforePause(3)
{
    // outer sclera (used for collision)
    shape.setRadius(30.f);
    shape.setOrigin(sf::Vector2f(30.f, 30.f));
    shape.setPosition(pos);
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::Red); // subtle rim color

    // iris (centered on the eye)
    float irisRadius = 15.f;
    iris.setRadius(irisRadius);
    iris.setOrigin(sf::Vector2f(irisRadius, irisRadius));
    iris.setPosition(pos);
    iris.setFillColor(sf::Color(30, 144, 255)); // DodgerBlue iris

    // pupil (on top of the iris)
    float pupilRadius = 6.f;
    pupil.setRadius(pupilRadius);
    pupil.setOrigin(sf::Vector2f(pupilRadius, pupilRadius));
    pupil.setPosition(pos);
    pupil.setFillColor(sf::Color::Black);

    // small highlight to give the eye some sheen
    float hlRadius = 3.f;
    highlight.setRadius(hlRadius);
    highlight.setOrigin(sf::Vector2f(hlRadius, hlRadius));
    // place highlight slightly offset on the upper-left of the pupil
    highlight.setPosition(pos + sf::Vector2f(-irisRadius * 0.35f, -irisRadius * 0.35f));
    highlight.setFillColor(sf::Color(255, 255, 255, 200));

    // interval initial aléatoire 0.5-1.5s
    shootInterval = 0.5f + (std::rand() % 100) / 100.f;
}

void Enemy::update(float dt, BulletManager& mgr) {
    shootTimer += dt;

    // keep the decorative parts aligned with pos (in case position changes later)
    shape.setPosition(pos);
    iris.setPosition(pos);
    pupil.setPosition(pos);
    highlight.setPosition(pos + sf::Vector2f(-iris.getRadius() * 0.35f, -iris.getRadius() * 0.35f));

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
    // draw in order: sclera, iris, pupil, highlight
    rt.draw(shape);
    rt.draw(iris);
    rt.draw(pupil);
    rt.draw(highlight);
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