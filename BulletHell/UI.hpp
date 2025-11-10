#pragma once
#include <SFML/Graphics.hpp>

class UI {
public:
    UI();
    void init();
    void update(float dt, int score);
    void draw(sf::RenderTarget& rt);

private:
    sf::Font font;
    sf::Text scoreText;
};
