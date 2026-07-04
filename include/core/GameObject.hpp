#pragma once
#include <SFML/Graphics.hpp>

// Abstract base for everything that lives in the game world and needs to be
// updated and rendered every frame (Player, all Platform types).
// Pure virtual functions force dynamic binding through GameObject* / &.
class GameObject
{
public:
    explicit GameObject(const sf::Texture &texture);
    virtual ~GameObject() = default;

    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow &window);

    virtual sf::FloatRect getBounds() const;

    void setPosition(sf::Vector2f position);
    sf::Vector2f getPosition() const;

protected:
    sf::Sprite sprite;
};
