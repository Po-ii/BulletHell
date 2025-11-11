#include "pch.h"
#include "UI.hpp"
#include <sstream>

UI::UI() : scoreText(font, "") {
    // default constructor -- nothing to do here
}

void UI::init() {
    if (!font.openFromFile("assets/DejaVuSans.ttf")) {
        // Handle error: font file could not be loaded
        // For now, throw an exception or log an error
        throw std::runtime_error("Failed to load font: assets/DejaVuSans.ttf");
    }
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setPosition(sf::Vector2f(10.f, 10.f));
    scoreText.setFillColor(sf::Color::White);
}

void UI::update(float, int score) {
    std::ostringstream ss;
    ss << "Score: " << score;
    scoreText.setString(ss.str());
}

void UI::draw(sf::RenderTarget& rt) {
    rt.draw(scoreText);
}