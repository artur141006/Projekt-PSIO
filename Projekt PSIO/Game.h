#pragma once

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"
#include "PowerUp.h"
#include <vector>
#include <string>
#include <memory>
#include <random>

struct PlayerData {
    std::string name;
    int score;
};

template<typename T> //klasa generyczna
class RandomGenerator {
public:
    RandomGenerator(T min, T max) : distribution(min, max) {}

    T getRandom() {
        return distribution(engine);
    }

private:
    std::random_device rd;
    std::default_random_engine engine{ rd() };
    std::uniform_int_distribution<T> distribution;
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
    int newHealth = 100;
    RandomGenerator<int> randGen{ 0, 2 };

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