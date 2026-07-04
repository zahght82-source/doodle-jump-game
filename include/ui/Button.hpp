#pragma once
#include <SFML/Graphics.hpp>

// A clickable button: a sprite (from a button texture asset) with a
// centered text label on top. Used by the Menu and GameOver screens.
class Button
{
public:
    Button(const sf::Texture &texture, const sf::Font &font, const std::string &label, unsigned int characterSize = 28);

    void setPosition(sf::Vector2f position);
    void setScale(sf::Vector2f scale);

    void render(sf::RenderWindow &window);

    // True if the given point (e.g. mouse position) is within the
    // button's current on-screen bounds.
    bool isHovered(sf::Vector2i point) const;

    sf::FloatRect getBounds() const;

private:
    sf::Sprite sprite;
    sf::Text text;
};
