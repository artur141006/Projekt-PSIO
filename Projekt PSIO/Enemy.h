#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Entity.h"
#include "Player.h"

class Enemy : public Entity {
private:
    int health;
    float speed;
    sf::Texture texture;
    sf::Sprite sprite;
    Player& player;

public:
    Enemy(int initHealth, float initSpeed, Player& playerRef);
    void update() override;
    void takeDamage(int damage);
    int getHealth() const;
    void setSpeed(float newSpeed);
    void setPosition(float x, float y);
    void draw(sf::RenderWindow& window) override;
    const sf::Sprite& getSprite() const;
    Enemy& operator=(const Enemy& other) {
        if (this != &other) {
            health = other.health;
            speed = other.speed;
            texture = other.texture;
            sprite.setTexture(texture);
            player = other.player;
        }
        return *this;
    }
    bool operator==(const Enemy& other) const {
        return this == &other;
    }
    ~Enemy() {};
};