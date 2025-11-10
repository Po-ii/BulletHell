#pragma once
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "UI.hpp"
#include "LaserManager.hpp"

class Game {
public:
    enum class State { MainMenu, Playing, GameOver };
    LaserManager lasers;
    Game();
    ~Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

    void updateMenu();
    void startGame();
    void endGame(bool victory);

    sf::RenderWindow window;
    State state;

    Player player;
    BulletManager bullets;
    std::vector<Enemy> enemies;

    UI ui;
    sf::Clock clock;

    float spawnTimer;
    float spawnInterval;
    int score;

    std::string gameOverMessage;
    bool lastVictory = false; // track outcome to colorize final screen

    // Countdown timer (chrono) for Playing state
    float timeLeftSeconds = 0.f;                 // current remaining time in seconds
    static constexpr float initialTimeSeconds = 5.f * 60.f; // 5 minutes

    // Repeated, scrolling background
    sf::Texture bgTexture;
    sf::RectangleShape bgRect;
    float bgScrollOffset = 0.f;      // vertical offset in texture pixels
    float bgScrollSpeed = 60.f;      // pixels per second (tweak as desired)
    bool bgLoaded = false;
};