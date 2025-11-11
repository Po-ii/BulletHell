#include "pch.h"
#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <limits>

Player::Player()
{
    pos = { 480.f, 620.f };
    speed = 300.f;

    // collision circle (kept for collisions)
    shape.setRadius(8.f);
    shape.setOrigin(sf::Vector2f(8.f, 8.f));

    // Build a simple triangular spaceship (nose up) scaled up
    ship.setPointCount(3);
    // local coordinates produce a ship that fits roughly inside 16x16 (diameter = 16)
    ship.setPoint(0, sf::Vector2f(0.f, -16.f));   // nose
    ship.setPoint(1, sf::Vector2f(-16.f, 16.f));  // left wing
    ship.setPoint(2, sf::Vector2f(16.f, 16.f));   // right wing
    ship.setFillColor(sf::Color(100, 220, 255)); // ship body color
    ship.setOutlineThickness(1.f);
    ship.setOutlineColor(sf::Color::Black);

    // compute bounding box from the convex shape points (avoids using FloatRect members)
    float minX = std::numeric_limits<float>::infinity();
    float minY = std::numeric_limits<float>::infinity();
    float maxX = -std::numeric_limits<float>::infinity();
    float maxY = -std::numeric_limits<float>::infinity();

    for (std::size_t i = 0; i < ship.getPointCount(); ++i) {
        sf::Vector2f p = ship.getPoint(i);
        minX = std::min(minX, p.x);
        minY = std::min(minY, p.y);
        maxX = std::max(maxX, p.x);
        maxY = std::max(maxY, p.y);
    }

    float centerX = (minX + maxX) * 0.5f;
    float centerY = (minY + maxY) * 0.5f;
    ship.setOrigin(sf::Vector2f(centerX, centerY));
    ship.setPosition(pos);
}

void Player::update(float dt) {
    // use scancodes for WASD
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) moveUp(dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) moveDown(dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) moveLeft(dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) moveRight(dt);

    if (shootTimer > 0.f) shootTimer -= dt;

    // clamp to screen area (assuming 960x720)
    pos.x = std::clamp(pos.x, 10.f, 960.f - 10.f);
    pos.y = std::clamp(pos.y, 10.f, 720.f - 10.f);

    // keep visual shapes aligned with position
    ship.setPosition(pos);
    shape.setPosition(pos);
}

void Player::draw(sf::RenderTarget& rt) const {
    // draw spaceship first, then (optionally) collision circle for debugging
    rt.draw(ship);
    // Uncomment to visualize collision circle:
    // rt.draw(shape);
}

void Player::reset(const sf::Vector2f& p) {
    // reset position and gameplay-related state (health, shooting cooldown)
    pos = p;
    hp = Player::DefaultHP; // use centralized default
    shootTimer = 0.f;    // allow immediate shooting
    // visual shapes will be updated in the next update() call
    ship.setPosition(pos);
    shape.setPosition(pos);
}

void Player::moveUp(float dt) { pos.y -= speed * dt; }
void Player::moveDown(float dt) { pos.y += speed * dt; }
void Player::moveLeft(float dt) { pos.x -= speed * dt; }
void Player::moveRight(float dt) { pos.x += speed * dt; }