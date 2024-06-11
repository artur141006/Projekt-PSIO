#include "Player.h"
#include "Enemy.h"
#include "PowerUp.h"

Player::Player() : lives(3), score(0), damage(50), speed(1.0f), powerLevel(1), speedMultiplier(1.0f), extraLives(0) {
    if (!texture.loadFromFile("player.png")) {
        std::cerr << "Error loading player texture\n";
    }
    else {
        std::cout << "Player texture loaded successfully\n";
    }
    sprite.setTexture(texture);
    sprite.setScale(0.1f, 0.1f);
    sprite.setPosition(950, 550);
}

void Player::update(PowerUp& powerUp) {
    handleInput();
    updateArrows();
    checkPowerUpCollision(powerUp);
}

void Player::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        sprite.move(0, -speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        sprite.move(-speed, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        sprite.move(0, speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        sprite.move(speed, 0);
    }
}

void Player::updateArrows() {
    for (auto& arrow : arrows) {
        arrow.update();
    }
}

void Player::shoot(float angle) {
    float playerX = sprite.getPosition().x;
    float playerY = sprite.getPosition().y;
    float arrowX = playerX + (sprite.getGlobalBounds().width / 2);
    float arrowY = playerY + (sprite.getGlobalBounds().height / 2);
    arrows.push_back(Arrow(angle, arrowX, arrowY));
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font\n";
        return;
    }

    sf::Text livesText("Lives: " + std::to_string(lives), font, 20);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(20, 20);
    window.draw(livesText);

    sf::Text scoreText("Score: " + std::to_string(score), font, 20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(20, 50);
    window.draw(scoreText);

    for (auto& arrow : arrows) {
        arrow.draw(window);
    }
}

void Player::decreaseLives() {
    lives--;
}

void Player::increaseScore(int points) {
    score += points;
}

const sf::Sprite& Player::getSprite() const {
    return sprite;
}

void Player::checkArrowCollision(std::unique_ptr<Enemy>& enemy, bool& collidedWithPlayer) {
    for (auto it = arrows.begin(); it != arrows.end();) {
        if (it->getSprite().getGlobalBounds().intersects(enemy->getSprite().getGlobalBounds())) {
            enemy->takeDamage(damage);
            if (enemy->getHealth() <= 0) {
                increaseScore(1);
            }
            it = arrows.erase(it);
        }
        else {
            ++it;
        }
    }

    if (getSprite().getGlobalBounds().intersects(enemy->getSprite().getGlobalBounds())) {
        decreaseLives();
        collidedWithPlayer = true;
    }
}

void Player::checkPowerUpCollision(PowerUp& powerUp) {
    if (sprite.getGlobalBounds().intersects(powerUp.getSprite().getGlobalBounds())) {
        int type = powerUp.getType();
        if (type == 0) {
            extraLives++;
        }
        else if (type == 1) {
            powerLevel++;
        }
        else if (type == 2) {
            speedMultiplier += 0.5f;
        }
    }
}

int Player::getScore() const {
    return score;
}

int Player::getLives() const {
    return lives;
}

void Player::reset() {
    lives = 3;
    score = 0;
    sprite.setPosition(950, 550);
    arrows.clear();
}

void Player::increaseLives(int numLives) {
    lives += numLives;
}

void Player::increasePowerLevel(int numLevels) {
    damage += (numLevels * 20);
}

void Player::increaseSpeedMultiplier(float multiplier) {
    speed += multiplier;
}