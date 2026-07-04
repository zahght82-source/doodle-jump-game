#pragma once
#include "core/GameObject.hpp"

// Base class for all platform types (Normal, Moving, Breakable).
// The rest of the game only ever touches platforms through Platform*,
// so landing behaviour dispatches virtually -- no per-type branching
// anywhere else in the codebase (this is what the collision system
// relies on).
class Platform : public GameObject
{
public:
    explicit Platform(const sf::Texture &texture);
    virtual ~Platform() = default;

    // Called by the collision system when the player lands on top of
    // this platform while moving downward. Returns the vertical
    // velocity the player should be given immediately after landing
    // (e.g. JUMP_VELOCITY, SPRING_JUMP_VELOCITY, or 0 if no bounce --
    // Breakable platforms return 0 and instead start falling).
    virtual float onPlayerLand() = 0;

    // Whether the player can currently stand on this platform.
    // A Breakable platform stops supporting the player once broken.
    virtual bool isSolid() const;

    // Whether this platform should be removed from the world
    // (e.g. a broken platform that has fallen off-screen).
    virtual bool isExpired() const;

protected:
    bool solid = true;
    bool expired = false;
};
