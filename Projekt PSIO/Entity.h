#pragma once

#include <SFML/Graphics.hpp>

class Entity {
protected:
    sf::Texture texture;

public:
    sf::Sprite sprite;
    virtual void update() = 0; //metoda wirtualna
    virtual void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
    virtual ~Entity() {}
};