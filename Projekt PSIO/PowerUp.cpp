#include "PowerUp.h"

PowerUp::PowerUp(float x, float y, int type) : type(type) {
    if (!texture.loadFromFile("powerup.png")) {
        std::cerr << "Error loading powerup texture\n";
    }
    else {
        std::cout << "Powerup texture loaded successfully\n";
    }
    sprite.setTexture(texture);
    sprite.setScale(0.05f, 0.05f);
    sprite.setPosition(x, y);
}

int PowerUp::getType() const {
    return type;
}

void PowerUp::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

const sf::Sprite& PowerUp::getSprite() const {
    return sprite;
}

void PowerUp::reset() {
    sprite.setPosition(-1000, -1000);
}