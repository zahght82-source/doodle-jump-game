#include "ui/Button.hpp"

Button::Button(const sf::Texture &texture, const sf::Font &font, const std::string &label, unsigned int characterSize)
    : sprite(texture),
      text(font, label, characterSize)
{
    sf::FloatRect spriteBounds = sprite.getLocalBounds();
    sprite.setOrigin({spriteBounds.size.x / 2.f, spriteBounds.size.y / 2.f});

    text.setFillColor(sf::Color::White);
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({textBounds.position.x + textBounds.size.x / 2.f,
                     textBounds.position.y + textBounds.size.y / 2.f});
}

void Button::setPosition(sf::Vector2f position)
{
    sprite.setPosition(position);
    text.setPosition(position);
}

void Button::setScale(sf::Vector2f scale)
{
    sprite.setScale(scale);
}

void Button::render(sf::RenderWindow &window)
{
    window.draw(sprite);
    window.draw(text);
}

bool Button::isHovered(sf::Vector2i point) const
{
    return sprite.getGlobalBounds().contains({static_cast<float>(point.x), static_cast<float>(point.y)});
}

sf::FloatRect Button::getBounds() const
{
    return sprite.getGlobalBounds();
}
