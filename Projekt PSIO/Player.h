#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Arrow.h"
#include "PowerUp.h"

class Enemy;

class Player {
private:
    int lives;
    int score;
    int damage;
    float speed;
    int powerLevel;
    float speedMultiplier;
    int extraLives;
    sf::Texture texture;
    sf::Sprite sprite;
    std::vector<Arrow> arrows;

public:
    Player();
    void update(PowerUp& powerUp);
    void handleInput();
    void updateArrows();
    void shoot(float angle);
    void draw(sf::RenderWindow& window);
    void checkPowerUpCollision(PowerUp& powerUp);
    void decreaseLives();
    void increaseScore(int points);
    void checkArrowCollision(std::unique_ptr<Enemy>& enemy, bool& collidedWithPlayer);
    int getScore() const;
    int getLives() const;
    void increaseLives(int numLives);
    void increasePowerLevel(int numLevels);
    void increaseSpeedMultiplier(float multiplier);
    void reset();
    const sf::Sprite& getSprite() const;
};

#endif