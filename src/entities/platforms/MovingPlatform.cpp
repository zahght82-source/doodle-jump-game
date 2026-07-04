#include "entities/platforms/MovingPlatform.hpp"
#include "utils/Constants.hpp"

MovingPlatform::MovingPlatform(const sf::Texture &texture, unsigned int windowWidth)
    : Platform(texture),
      velocityX(Constants::MOVING_PLATFORM_SPEED),
      windowWidth(windowWidth)
{
}

void MovingPlatform::update(float deltaTime)
{
    sf::Vector2f pos = getPosition();
    float halfWidth = getBounds().size.x / 2.f;

    pos.x += velocityX * deltaTime;

    // Bounce off the left/right screen edges.
    if (pos.x - halfWidth <= 0.f)
    {
        pos.x = halfWidth;
        velocityX = -velocityX;
    }
    else if (pos.x + halfWidth >= static_cast<float>(windowWidth))
    {
        pos.x = static_cast<float>(windowWidth) - halfWidth;
        velocityX = -velocityX;
    }

    setPosition(pos);
}

float MovingPlatform::onPlayerLand()
{
    return Constants::JUMP_VELOCITY;
}
