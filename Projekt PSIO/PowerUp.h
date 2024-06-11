#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class PowerUp {
private:
    int type;
    sf::Texture texture;
    sf::Sprite sprite;

public:
    PowerUp() = default;
    PowerUp(float x, float y, int type);
    int getType() const;
    void draw(sf::RenderWindow& window);
    void reset();
    const sf::Sprite& getSprite() const;
};