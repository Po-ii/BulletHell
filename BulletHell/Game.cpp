#include "pch.h"
#include "Game.hpp"
#include <cmath>
#include <sstream>
#include <SFML/System.hpp>
#include <iomanip>

typedef unsigned int Uint32;

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

        // update countdown timer
        if (timeLeftSeconds > 0.f) {
            timeLeftSeconds -= dt;
            if (timeLeftSeconds <= 0.f) {
                timeLeftSeconds = 0.f;
                endGame(true); // survived the timer -> win
                return; // avoid further game logic this frame
            }
        }

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
                e.takeDamage(3);
                if (e.getHP() <= 0) endGame(true); // victoire
            }
        }

        // Collision bullets -> joueur
        if (bullets.checkCollision(player.getPosition(), 16.f)) {
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

    // draw repeated, scrolling background except on the Main Menu / intro screen
    if (bgLoaded && state != State::MainMenu) {
        window.draw(bgRect);
    }

    sf::Font f;
    if (!f.openFromFile("assets/DejaVuSans.ttf")) {
        // Handle error: font file not found or failed to load
        // For now, just return to avoid drawing text with an invalid font
        return;
    }

    // helper: compute text block width/height without accessing FloatRect members
    auto computeTextDims = [](const sf::Text& txt) -> sf::Vector2f {
        const sf::Font* font = &txt.getFont();
        if (!font) return { 0.f, 0.f };

        const sf::String& s = txt.getString();
        unsigned int charSize = txt.getCharacterSize();

        float maxW = 0.f;
        float curW = 0.f;
        unsigned int lines = 1;
        uint32_t prev = 0;

        for (std::size_t i = 0; i < s.getSize(); ++i) {
            Uint32 c = s[i];
            if (c == '\n') {
                if (curW > maxW) maxW = curW;
                curW = 0.f;
                prev = 0;
                ++lines;
                continue;
            }

            const sf::Glyph& g = font->getGlyph(c, charSize, false);
            if (prev) curW += font->getKerning(prev, c, charSize);
            curW += g.advance;
            prev = c;
        }
        if (curW > maxW) maxW = curW;

        float height = font->getLineSpacing(charSize) * static_cast<float>(lines);
        return { maxW, height };
    };

    // helper to center text horizontally at given y using computed dims
    auto centerText = [&](sf::Text& txt, float y) {
        sf::Vector2f d = computeTextDims(txt);
        txt.setOrigin(sf::Vector2f(d.x * 0.5f, d.y * 0.5f));
        txt.setPosition(sf::Vector2f(static_cast<float>(window.getSize().x) * 0.5f, y));
    };

    sf::Text t(f, "");

    switch (state) {
    case State::MainMenu: {
        // Title (cyan)
        t.setString("EyeStorm");
        t.setCharacterSize(64);
        t.setFillColor(sf::Color(100, 220, 255)); // cyan
        centerText(t, static_cast<float>(window.getSize().y) * 0.22f);
        window.draw(t);

        // Short synopsis (multi-line) (light gray)
        t.setCharacterSize(20);
        t.setFillColor(sf::Color(200, 200, 200));
        t.setString("Synopsis:\nYou are the last pilot defending humanity\ntrying to make it back to your home planet Veyra.\nSuddenly attacked by an onslaught of eye-like enemies.\nDodge patterns and eliminate the threat.");
        // center the block a bit lower
        centerText(t, static_cast<float>(window.getSize().y) * 0.48f);
        window.draw(t);

        // Controls / prompt (yellow)
        t.setCharacterSize(18);
        t.setFillColor(sf::Color(255, 215, 0)); // gold
        t.setString("Controls: ZQSD (Azety) or WASD (Qwerty) to move  -  SPACE to shoot\nPress SPACE to start");
        centerText(t, static_cast<float>(window.getSize().y) * 0.78f);
        window.draw(t);
        break;
    }

    case State::Playing:
        bullets.render(window);
        lasers.render(window);
        for (auto& e : enemies) e.draw(window);
        player.draw(window);

        // draw countdown timer (top center)
        {
            std::ostringstream ss;
            int total = static_cast<int>(std::ceil(timeLeftSeconds));
            int minutes = total / 60;
            int seconds = total % 60;
            ss << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;
            t.setCharacterSize(24);
            t.setFillColor(sf::Color(255, 215, 0)); // gold
            t.setString(ss.str());
            centerText(t, 30.f);
            window.draw(t);
        }

        // UI
        t.setCharacterSize(20);

        // Player HP (green)
        t.setFillColor(sf::Color(0, 200, 0));
        t.setString("Player HP: " + std::to_string(player.getHP()));
        t.setPosition(sf::Vector2f(10.f, 10.f)); window.draw(t);

        // Enemy HP (orange)
        t.setFillColor(sf::Color(255, 165, 0));
        t.setString("Enemy HP: " + std::to_string(enemies[0].getHP()));
        t.setPosition(sf::Vector2f(700.f, 10.f)); window.draw(t);

        break;

    case State::GameOver: {
        // draw scene behind final screen (keep bullets/enemies/player visible)
        bullets.render(window);
        lasers.render(window);
        for (auto& e : enemies) e.draw(window);
        player.draw(window);

        // overlay dark translucent rectangle
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
        overlay.setFillColor(sf::Color(0, 0, 0, 160));
        window.draw(overlay);

        // Result title (centered) - color depends on outcome
        t.setCharacterSize(48);
        if (lastVictory) t.setFillColor(sf::Color(100, 255, 150)); // light green
        else t.setFillColor(sf::Color(255, 100, 100)); // light red
        t.setString(gameOverMessage);
        centerText(t, static_cast<float>(window.getSize().y) * 0.38f);
        window.draw(t);

        // Score / summary (light gray)
        t.setCharacterSize(20);
        t.setFillColor(sf::Color(200,200,200));
        t.setString("Final Score: " + std::to_string(score));
        centerText(t, static_cast<float>(window.getSize().y) * 0.52f);
        window.draw(t);

        // Restart prompt (yellow)
        t.setCharacterSize(18);
        t.setFillColor(sf::Color(255, 215, 0));
        t.setString("Press SPACE to restart");
        centerText(t, static_cast<float>(window.getSize().y) * 0.7f);
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

    // start the 5-minute countdown
    timeLeftSeconds = Game::initialTimeSeconds;

    // Reset ennemi
    enemies.clear();
    enemies.emplace_back(sf::Vector2f(480.f, 120.f));
}

void Game::endGame(bool victory) {
    state = State::GameOver;
    lastVictory = victory;
    gameOverMessage = victory ? "Congratulations! You made it safe back home." : "GAME OVER! YOU DIED!";
}