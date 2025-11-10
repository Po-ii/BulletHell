#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    Player();
    void update(float dt);
    void draw(sf::RenderTarget& rt) const;
    sf::Vector2f getPosition() const { return pos; }
    void reset(const sf::Vector2f& p);

    // direct movement helpers (if you want to use switch-case controls)
    void moveUp(float dt);
    void moveDown(float dt);
    void moveLeft(float dt);
    void moveRight(float dt);

    void takeDamage(int dmg) { hp -= dmg; if (hp < 0) hp = 0; }
    int getHP() const { return hp; }

    bool canShoot() const { return shootTimer <= 0.f; }
    void shoot() { shootTimer = shootCooldown; }

private:
    sf::Vector2f pos;
    sf::CircleShape shape;   // collision / fallback circle
    float speed;

    // draw a simple spaceship using SFML primitives (no assets)
    sf::ConvexShape ship;

    int hp = 300;

    float shootTimer = 0.f;
    const float shootCooldown = 0.3f;
};