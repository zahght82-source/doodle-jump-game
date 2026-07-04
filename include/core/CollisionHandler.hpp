#pragma once
#include "entities/Player.hpp"
#include "core/PlatformManager.hpp"

// Resolves player-vs-platform collisions. Kept as a small set of free
// functions rather than folding this logic into Player or Platform,
// so neither class needs to know about the other's internals.
namespace CollisionHandler
{
    // Checks the player against all platforms and applies landing effects
    // (bounce velocity, spring boost, breaking). Only triggers when the
    // player is moving downward and lands on top of a solid platform --
    // landing from below passes through, per the game rules.
    void resolvePlatformCollisions(Player &player, PlatformManager &platformManager);
}
