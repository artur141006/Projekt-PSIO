#include "Arrow.h"
#define M_PI acos(-1.0)

Arrow::Arrow(float angle, float startX, float startY) : speed(5.f) {
    direction.x = std::cos(angle * M_PI / 180.f);
    direction.y = std::sin(angle * M_PI / 180.f);
    if (!texture.loadFromFile("arrow.png")) {
        std::cerr << "Error loading arrow texture\n";
    }
    else {
        std::cout << "Arrow texture loaded successfully\n";
    }
    sprite.setTexture(texture);
    sprite.setScale(0.05f, 0.05f);
    sprite.setPosition(startX, startY);
}

void Arrow::update() {
    sprite.move(speed * direction);
}

void Arrow::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

const sf::Sprite& Arrow::getSprite() const {
    return sprite;
}