#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"
#include "PowerUp.h"
#include <vector>
#include <string>
#include <memory>

struct PlayerData {
    std::string name;
    int score;
};

class Game {
private:
    sf::RenderWindow window;
    Player player;
    PowerUp powerUp;
    sf::Font font;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<PlayerData> scores;
    sf::Text powerUpText;
    sf::Clock powerUpClock;
    sf::Clock pauseClock;
    bool isPaused;
    float newSpeed = 0.5;

public:
    Game();
    void generatePowerUp();
    void run();
    void handleEvents();
    void update();
    void render();
    void initializeEnemies();
    void resetEnemies();
    void gameOver();
    void loadScores();
    void saveScores();
};

#endif