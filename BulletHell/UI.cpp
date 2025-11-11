#include "pch.h"
#include "UI.hpp"
#include <sstream>
#include <iostream>

UI::UI()
    : titleFont()
    , bodyFont()
    , scoreText(bodyFont, "") // initialize with a font and empty string
{
    // nothing here; fonts loaded in init()
}

void UI::init() {
    // Try to load VT323 for body text first (preferred).
    // If missing, fall back to Press Start 2P for body text so UI still shows.
    bool bodyLoaded = bodyFont.openFromFile("assets/VT323-Regular.ttf");
    if (!bodyLoaded) {
        std::cerr << "Warning: assets/VT323-Regular.ttf not found, trying PressStart2P as fallback for body text.\n";
        bodyLoaded = bodyFont.openFromFile("assets/PressStart2P-Regular.ttf");
        if (!bodyLoaded) {
            throw std::runtime_error("Failed to load UI body font: assets/VT323-Regular.ttf or fallback assets/PressStart2P-Regular.ttf");
        }
    }

    // Load title font (Press Start 2P). If missing, use bodyFont as fallback.
    bool titleLoaded = titleFont.openFromFile("assets/PressStart2P-Regular.ttf");
    if (!titleLoaded) {
        std::cerr << "Warning: assets/PressStart2P-Regular.ttf not found, using body font as title fallback.\n";
    }

    // Initialize scoreText with the body font (already set in ctor), update other properties
    scoreText.setFont(bodyFont);
    scoreText.setCharacterSize(24); // increased size
    scoreText.setPosition(sf::Vector2f(10.f, 10.f));
    scoreText.setFillColor(sf::Color::White);
}

const sf::Font& UI::getTitleFont() const {
    if (!titleFont.getInfo().family.empty()) return titleFont;
    return bodyFont;
}

const sf::Font& UI::getBodyFont() const {
    return bodyFont;
}

void UI::update(float, int score) {
    std::ostringstream ss;
    ss << "Score: " << score;
    scoreText.setString(ss.str());
}

void UI::draw(sf::RenderTarget& rt) {
    rt.draw(scoreText);
}