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
    sf::CircleShape shape;
    float speed;

    // sprite asset
    sf::Texture texture;
    sf::Sprite sprite; // <- use default construction (no brace-init)

    bool spriteLoaded = false;

    int hp = 300;

    float shootTimer = 0.f;
    const float shootCooldown = 0.3f;
};