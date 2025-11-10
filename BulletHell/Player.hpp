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

private:
    sf::Vector2f pos;
    sf::CircleShape shape;
    float speed;
};