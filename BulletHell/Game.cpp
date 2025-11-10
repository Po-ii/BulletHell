#include "pch.h"
#include "Game.hpp"
#include <cmath>
#include <sstream>

Game::Game()
    : window(sf::VideoMode({ 960u, 720u }), "Bullet Hell - Mini Touhou")
    , state(State::MainMenu)
    , player()
    , bullets(4000) // pool size
    , spawnTimer(0.f)
    , spawnInterval(1.2f)
    , score(0)
{
    window.setFramerateLimit(60);
    ui.init();
    // create one enemy spawner
    enemies.emplace_back(sf::Vector2f(480.f, 120.f));
}

Game::~Game() = default;

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        float dt = clock.restart().asSeconds();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    // SFML 3: window.pollEvent() returns an optional<sf::Event>
    while (auto eventOpt = window.pollEvent()) {
        const sf::Event& e = *eventOpt;

        if (e.is<sf::Event::Closed>()) {
            window.close();
            continue;
        }

        if (e.is<sf::Event::KeyPressed>()) {
            const auto* key = e.getIf<sf::Event::KeyPressed>();
            if (!key) // defensive, but should be present when is<KeyPressed>() is true
                continue;

            if (key->code == sf::Keyboard::Key::Escape) {
                window.close();
                continue;
            }

            if (state == State::MainMenu && key->code == sf::Keyboard::Key::Space) {
                startGame();
                continue;
            }

            if (state == State::GameOver && key->code == sf::Keyboard::Key::Space) {
                startGame();
                continue;
            }
        }
    }
}

void Game::update(float dt) {
    switch (state) {
    case State::MainMenu: updateMenu(); break;
    case State::Playing: {
        // player input & movement
        player.update(dt);

        // spawn patterns from enemies
        spawnTimer += dt;
        if (spawnTimer >= spawnInterval) {
            spawnTimer = 0.f;
            for (auto& e : enemies) {
                // example: alternating circular bursts and spiral
                static bool alt = false;
                if (!alt) e.spawnCircular(bullets, 32, 140.f);
                else      e.spawnSpiral(bullets, 40, 100.f, 0.04f);
                alt = !alt;
            }
        }

        // update bullets
        bullets.update(dt);

        // update enemies (they could move)
        for (auto& e : enemies) e.update(dt);

        // collisions: bullets vs player
        if (bullets.checkCollision(player.getPosition(), 8.f)) {
            endGame();
        }

        // simple scoring by time
        score += int(dt * 10.f);

        // UI update
        ui.update(dt, score);

        break;
    }
    case State::GameOver:
        // could animate gameover screen
        break;
    }
}

void Game::render() {
    window.clear(sf::Color(12, 12, 20));

    switch (state) {
    case State::MainMenu: renderMenu(); break;
    case State::Playing: {
        // draw playfield
        bullets.render(window);
        for (auto& e : enemies) e.draw(window);
        player.draw(window);
        ui.draw(window);
        break;
    }
    case State::GameOver: {
        // draw last frame overlay
        bullets.render(window);
        for (auto& e : enemies) e.draw(window);
        player.draw(window);
        ui.draw(window);
        // Game over text
        sf::Font f; f.openFromFile("assets/DejaVuSans.ttf");
        sf::Text t(f, "GAME OVER\nPress SPACE to restart", 36);
        t.setFillColor(sf::Color::White);
        t.setPosition(sf::Vector2f(220.f, 300.f));
        window.draw(t);
        break;
    }
    }

    window.display();
}

void Game::updateMenu() {
    // could animate background
}

void Game::renderMenu() {
    // draw title + instructions
    sf::Font f; f.openFromFile("assets/DejaVuSans.ttf");
    sf::Text title(f, "Mini Bullet Hell - Touhou-like\nPress SPACE to start\nWASD to move", 32);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(140.f, 260.f));
    window.draw(title);
}

void Game::startGame() {
    // reset everything
    state = State::Playing;
    player.reset(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y - 100.f));
    bullets.resetAll();
    spawnTimer = 0.f;
    score = 0;
}

void Game::endGame() {
    state = State::GameOver;
}
