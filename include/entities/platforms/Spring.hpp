#pragma once
#include "core/GameObject.hpp"

// A Spring is an attachment that a NormalPlatform may optionally own
// (composition, not inheritance -- a spring is a "thing on a platform",
// not a "kind of platform"). It renders itself slightly above the
// platform's surface and boosts the player's jump when landed on.
class Spring : public GameObject
{
public:
    explicit Spring(const sf::Texture &texture);

    void update(float deltaTime) override;

    // Positions the spring relative to the platform's top-center.
    void attachTo(sf::Vector2f platformPosition, sf::Vector2f platformSize);

    // Triggers the compressed ("squashed") visual, played briefly when
    // the player bounces off this spring, then automatically returns to
    // the normal stretched look.
    void compress();

private:
    float compressTimer = 0.f; // seconds remaining in the compressed visual
};
