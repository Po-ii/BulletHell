#pragma once
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "UI.hpp"

class Game {
public:
    enum class State { MainMenu, Playing, GameOver };

    Game();
    ~Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

    void updateMenu();
    void renderMenu();
    void startGame();
    void endGame();

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
};
