#pragma once
#include <SFML/Graphics.hpp>

// A clickable button backed by a sprite. The button texture assets
// (start_button.png, restart_button.png, menu_button.png) already have
// their label drawn into the image, so no separate text overlay is
// needed or drawn here.
class Button
{
public:
    explicit Button(const sf::Texture &texture);

    void setPosition(sf::Vector2f position);
    void setScale(sf::Vector2f scale);

    void render(sf::RenderWindow &window);

    // True if the given point (e.g. mouse position) is within the
    // button's current on-screen bounds.
    bool isHovered(sf::Vector2i point) const;

    sf::FloatRect getBounds() const;

private:
    sf::Sprite sprite;
};
