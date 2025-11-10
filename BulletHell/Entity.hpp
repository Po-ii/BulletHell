#pragma once
#include <SFML/Graphics.hpp>


class Entity {
public:
	virtual ~Entity() = default;
	virtual void update(float dt) = 0;
	virtual void draw(sf::RenderTarget& rt) const = 0;
	virtual sf::Vector2f getPosition() const = 0;
};