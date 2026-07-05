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
        float fallStep = Constants::BREAKABLE_FALL_SPEED * deltaTime;
        sf::Vector2f pos = getPosition();
        pos.y += fallStep;
        setPosition(pos);

        distanceFallen += fallStep;

        // Once it has fallen well past a full screen height it is surely
        // off-screen and can be removed by the platform manager. Using
        // an accumulated distance (rather than an absolute world Y)
        // keeps this correct no matter how far the view has scrolled up.
        if (distanceFallen > static_cast<float>(Constants::WINDOW_HEIGHT))
        {
            expired = true;
        }
    }
}

float BreakablePlatform::onPlayerLand()
{
    broken = true;
    solid = false; // won't support the player again after this landing
    // Per spec: landing on a Breakable platform does NOT bounce the
    // player. The platform starts falling and drags the player down
    // with it (handled by the collision system attaching the player's
    // vertical velocity to the platform's fall speed). The player can
    // still move left/right to escape onto a neighboring solid platform.
    return Constants::BREAKABLE_FALL_SPEED;
}

bool BreakablePlatform::isBroken() const
{
    return broken;
}
