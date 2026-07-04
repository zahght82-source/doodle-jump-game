#include "core/CollisionHandler.hpp"
#include "entities/platforms/NormalPlatform.hpp"

namespace CollisionHandler
{
    void resolvePlatformCollisions(Player &player, PlatformManager &platformManager)
    {
        // Only land while falling -- this is what makes passing through
        // a platform from below possible: on the way up there is no
        // downward velocity, so no landing check happens and the
        // player's bounding box simply passes through.
        if (!player.isFalling())
        {
            return;
        }

        sf::FloatRect playerBounds = player.getBounds();
        float playerLeft = playerBounds.position.x;
        float playerRight = playerBounds.position.x + playerBounds.size.x;
        float currentBottom = playerBounds.position.y + playerBounds.size.y;
        float previousBottom = player.getPreviousBottom();

        for (Platform *platform : platformManager.getPlatforms())
        {
            if (!platform->isSolid())
            {
                continue;
            }

            sf::FloatRect platformBounds = platform->getBounds();
            float platformTop = platformBounds.position.y;
            float platformLeft = platformBounds.position.x;
            float platformRight = platformBounds.position.x + platformBounds.size.x;

            // Horizontal overlap check.
            if (playerRight < platformLeft || playerLeft > platformRight)
            {
                continue;
            }

            // Landing is valid only if the player's feet crossed the
            // platform's top surface THIS frame: they were above it last
            // frame and are at or below it now. This is what prevents
            // landing from re-triggering every frame while the player is
            // simply resting/bouncing on the same platform, which was
            // causing the player to appear "stuck" and never fall.
            bool crossedTopThisFrame = previousBottom <= platformTop && currentBottom >= platformTop;
            if (!crossedTopThisFrame)
            {
                continue;
            }

            // dynamic_cast to check, at runtime, whether this specific
            // platform is a NormalPlatform with a spring -- required
            // because Spring is composed only inside NormalPlatform and
            // is not part of the common Platform interface.
            NormalPlatform *normal = dynamic_cast<NormalPlatform *>(platform);
            float newVelocity;
            if (normal != nullptr && normal->hasSpring() &&
                playerBounds.findIntersection(normal->getSpringBounds()).has_value())
            {
                newVelocity = normal->onSpringLand();
            }
            else
            {
                newVelocity = platform->onPlayerLand();
            }

            player.setVerticalVelocity(newVelocity);
            return; // land on at most one platform per frame
        }
    }
}
