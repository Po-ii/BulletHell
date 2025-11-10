#include "pch.h"
#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>

Player::Player() {
    pos = { 480.f, 620.f };
    speed = 320.f;
    shape.setRadius(8.f);
    shape.setOrigin(sf::Vector2f(8.f, 8.f));
    shape.setFillColor(sf::Color::Cyan);
}

void Player::update(float dt) {
    // use scancodes for WASD
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) moveUp(dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) moveDown(dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) moveLeft(dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) moveRight(dt);

    // clamp to screen area (assuming 960x720)
    pos.x = std::clamp(pos.x, 10.f, 960.f - 10.f);
    pos.y = std::clamp(pos.y, 10.f, 720.f - 10.f);
}

void Player::draw(sf::RenderTarget& rt) const {
    sf::CircleShape s = shape;
    s.setPosition(pos);
    rt.draw(s);
}

void Player::reset(const sf::Vector2f& p) {
    pos = p;
}

void Player::moveUp(float dt) { pos.y -= speed * dt; }
void Player::moveDown(float dt) { pos.y += speed * dt; }
void Player::moveLeft(float dt) { pos.x -= speed * dt; }
void Player::moveRight(float dt) { pos.x += speed * dt; }