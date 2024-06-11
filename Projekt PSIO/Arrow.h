#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Arrow {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    float speed;
    sf::Vector2f direction;

public:
    Arrow(float angle, float startX, float startY);
    void update();
    void draw(sf::RenderWindow& window);
    const sf::Sprite& getSprite() const;
};