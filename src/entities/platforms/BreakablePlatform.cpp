#include "entities/platforms/BreakablePlatform.hpp"
#include "utils/Constants.hpp"

BreakablePlatform::BreakablePlatform(const sf::Texture &texture)
    : Platform(texture)
{
}

void BreakablePlatform::update(float deltaTime)
{
    if (broken)
    {
        sf::Vector2f pos = getPosition();
        pos.y += Constants::BREAKABLE_FALL_SPEED * deltaTime;
        setPosition(pos);

        // Once it has fallen well past the bottom of the screen it can
        // be removed by the platform manager.
        if (pos.y > static_cast<float>(Constants::WINDOW_HEIGHT) + Constants::PLATFORM_HEIGHT)
        {
            expired = true;
        }
    }
}

float BreakablePlatform::onPlayerLand()
{
    broken = true;
    solid = false; // no longer supports the player after this landing
    return 0.f;     // no bounce -- player falls with it
}

bool BreakablePlatform::isBroken() const
{
    return broken;
}
