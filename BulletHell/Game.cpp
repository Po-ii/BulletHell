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

    // load repeated texture for scrolling background
    if (bgTexture.loadFromFile("assets/space_bg.jpg")) {
        bgLoaded = true;
        bgTexture.setRepeated(true);

        // size rectangle to window size (fills the view)
        auto wsize = window.getSize();
        bgRect.setSize(sf::Vector2f(static_cast<float>(wsize.x), static_cast<float>(wsize.y)));

        // attach texture and set initial texture rect
        bgRect.setTexture(&bgTexture);
        bgRect.setTextureRect(sf::IntRect({ 0, 0 }, { static_cast<int>(wsize.x), static_cast<int>(wsize.y) }));

        bgScrollOffset = 0.f;
    } else {
        bgLoaded = false; // silent fallback if missing
    }

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
    // update scrolling background offset (works in all states; restrict if needed)
    if (bgLoaded) {
        bgScrollOffset += bgScrollSpeed * dt;

        // wrap offset using texture height so it never grows unbounded
        unsigned int texH = bgTexture.getSize().y;
        if (texH > 0) {
            // compute integer offset in texture space
            int offsetY = static_cast<int>(std::fmod(bgScrollOffset, static_cast<float>(texH)));
            if (offsetY < 0) offsetY += static_cast<int>(texH);

            // textureRect's height equals window height to fill view; left is 0
            auto wsize = window.getSize();
            bgRect.setTextureRect(sf::IntRect({ 0, offsetY }, { static_cast<int>(wsize.x), static_cast<int>(wsize.y) }));
        }
    }

    switch (state) {
    case State::MainMenu: break;

    case State::Playing:
        player.update(dt);

        // Tir du joueur
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && player.canShoot()) {
            lasers.spawn(player.getPosition());
            player.shoot();
        }

        lasers.update(dt);

        for (auto& e : enemies) e.update(dt, bullets);

        bullets.update(dt);

        // Collisions laser -> ennemi
        for (auto& e : enemies) {
            if (lasers.checkCollision(e.getShape())) {
                e.takeDamage(2);
                if (e.getHP() <= 0) endGame(true); // victoire
            }
        }

        // Collision bullets -> joueur
        if (bullets.checkCollision(player.getPosition(), 8.f)) {
            player.takeDamage(5);
            if (player.getHP() <= 0) endGame(false); // défaite
        }

        score += int(dt * 10.f);
        ui.update(dt, score);
        break;

    case State::GameOver: break;
    }
}

void Game::render() {
    window.clear(sf::Color(12, 12, 20));

    // draw repeated, scrolling background first
    if (bgLoaded) {
        window.draw(bgRect);
    }

    sf::Font f;
    if (!f.openFromFile("assets/DejaVuSans.ttf")) {
        // Handle error: font file not found or failed to load
        // For now, just return to avoid drawing text with an invalid font
        return;
    }
    sf::Text t(f, "");

    switch (state) {
    case State::MainMenu: {
        t.setString("Mini Bullet Hell\nPress SPACE to start\nZQSD or WASD to move\n Space to shoot");
        t.setCharacterSize(32);
        t.setFillColor(sf::Color::White);
        t.setPosition(sf::Vector2f(180.f, 260.f));
        window.draw(t);
        break;
    }

    case State::Playing:
        bullets.render(window);
        lasers.render(window);
        for (auto& e : enemies) e.draw(window);
        player.draw(window);

        // UI
        t.setCharacterSize(20);
        t.setFillColor(sf::Color::White);

        t.setString("Player HP: " + std::to_string(player.getHP()));
        t.setPosition(sf::Vector2f(10.f, 10.f)); window.draw(t);

        t.setString("Enemy HP: " + std::to_string(enemies[0].getHP()));
        t.setPosition(sf::Vector2f(700.f, 10.f)); window.draw(t);

        break;

    case State::GameOver: {
        bullets.render(window);
        lasers.render(window);
        for (auto& e : enemies) e.draw(window);
        player.draw(window);

        t.setString(gameOverMessage);
        t.setCharacterSize(36);
        t.setFillColor(sf::Color::White);
        t.setPosition(sf::Vector2f(200.f, 300.f));
        window.draw(t);
        break;
    }
    }

    window.display();
}

void Game::updateMenu() {
    // could animate background
}

void Game::startGame() {
    state = State::Playing;
    player.reset(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y - 100.f));
    bullets.resetAll();
    lasers.update(0.f);
    spawnTimer = 0.f;
    score = 0;

    // Reset ennemi
    enemies.clear();
    enemies.emplace_back(sf::Vector2f(480.f, 120.f));
}

void Game::endGame(bool victory) {
    state = State::GameOver;
    gameOverMessage = victory ? "YOU WIN! Press SPACE to restart" : "GAME OVER! Press SPACE to restart";
}
