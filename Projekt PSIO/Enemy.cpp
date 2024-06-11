#include "Enemy.h"
#include "Entity.h"

Enemy::Enemy(int initHealth, float initSpeed, Player& playerRef) : health(initHealth), speed(initSpeed), player(playerRef) {
    if (!texture.loadFromFile("enemy.png")) {
        std::cerr << "Error loading enemy texture\n";
    }
    else {
        std::cout << "Enemy texture loaded successfully\n";
    }
    sprite.setTexture(texture);
    sprite.setScale(0.1f, 0.1f);
    sprite.setPosition(rand() % 1900, rand() % 900);
}

void Enemy::update() {
    sf::Vector2f playerPosition = player.getSprite().getPosition();
    sf::Vector2f enemyPosition = sprite.getPosition();
    sf::Vector2f direction = playerPosition - enemyPosition;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    direction /= length;
    sprite.move(speed * direction);
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        sprite.setPosition(-900, -900);
        health = 0;
    }
}

int Enemy::getHealth() const {
    return health;
}

void Enemy::setSpeed(float newSpeed) {
    speed = newSpeed;
}

void Enemy::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

const sf::Sprite& Enemy::getSprite() const {
    return sprite;
}