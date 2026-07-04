#include "core/GameObject.hpp"

GameObject::GameObject(const sf::Texture &texture)
    : sprite(texture)
{
    // Center the origin so setPosition() places the sprite by its center,
    // not its default top-left corner. Uses local bounds (unrotated,
    // unscaled size) which is what we want for origin calculation.
    sf::FloatRect localBounds = sprite.getLocalBounds();
    sprite.setOrigin({localBounds.size.x / 2.f, localBounds.size.y / 2.f});
}

void GameObject::render(sf::RenderWindow &window)
{
    window.draw(sprite);
}

sf::FloatRect GameObject::getBounds() const
{
    return sprite.getGlobalBounds();
}

void GameObject::setPosition(sf::Vector2f position)
{
    sprite.setPosition(position);
}

sf::Vector2f GameObject::getPosition() const
{
    return sprite.getPosition();
}
