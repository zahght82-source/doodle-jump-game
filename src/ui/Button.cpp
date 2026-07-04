#include "ui/Button.hpp"

Button::Button(const sf::Texture &texture)
    : sprite(texture)
{
    sf::FloatRect spriteBounds = sprite.getLocalBounds();
    sprite.setOrigin({spriteBounds.size.x / 2.f, spriteBounds.size.y / 2.f});
}

void Button::setPosition(sf::Vector2f position)
{
    sprite.setPosition(position);
}

void Button::setScale(sf::Vector2f scale)
{
    sprite.setScale(scale);
}

void Button::render(sf::RenderWindow &window)
{
    window.draw(sprite);
}

bool Button::isHovered(sf::Vector2i point) const
{
    return sprite.getGlobalBounds().contains({static_cast<float>(point.x), static_cast<float>(point.y)});
}

sf::FloatRect Button::getBounds() const
{
    return sprite.getGlobalBounds();
}
