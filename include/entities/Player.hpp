#pragma once
#include "core/GameObject.hpp"

enum class FacingDirection
{
    Left,
    Right
};

// The player-controlled character. Falls under gravity, jumps when it
// lands on a solid platform, wraps around screen edges horizontally,
// and tracks how high it has climbed for scoring.
class Player : public GameObject
{
public:
    Player(const sf::Texture &leftTexture, const sf::Texture &rightTexture);

    void update(float deltaTime) override;

    void moveLeft();
    void moveRight();
    void stopHorizontal();

    // Applies an instantaneous vertical velocity (used after a platform
    // or spring landing).
    void setVerticalVelocity(float velocityY);

    bool isFalling() const; // true when moving downward (for collision checks)
    float getVerticalVelocity() const;

    // Bottom edge of the player's bounding box on the previous frame,
    // before this frame's movement was applied. Used by CollisionHandler
    // to detect that the player's feet crossed a platform's top surface
    // this frame, rather than merely overlapping it (which would also be
    // true on every subsequent frame while resting on a platform between
    // jumps, causing landing to re-trigger every frame).
    float getPreviousBottom() const;

    // Highest point (smallest y) reached so far, used for scoring.
    float getMaxHeightReached() const;

private:
    void handleScreenWrap(unsigned int windowWidth);
    void switchTexture(FacingDirection direction);

    const sf::Texture &leftTexture;
    const sf::Texture &rightTexture;
    FacingDirection facing = FacingDirection::Right;

    sf::Vector2f velocity{0.f, 0.f};
    float horizontalInput = 0.f;

    float maxHeightReached = 0.f; // smallest (highest) y the player has reached; starts unset
    bool maxHeightInitialized = false;
    float previousBottom = 0.f; // player's feet y-position before this frame's movement
};
