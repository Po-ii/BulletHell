#pragma once
#include <SFML/Graphics.hpp>

class UI {
public:
    UI();
    void init();
    void update(float dt, int score);
    void draw(sf::RenderTarget& rt);

    // Fonts exposed so other modules (Game) can reuse them
    const sf::Font& getTitleFont() const;
    const sf::Font& getBodyFont() const;

private:
    sf::Font titleFont;
    sf::Font bodyFont;
    sf::Text scoreText;
};
