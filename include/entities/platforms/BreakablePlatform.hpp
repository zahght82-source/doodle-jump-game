#pragma once
#include "entities/platforms/Platform.hpp"

// Breaks on first contact: stops supporting the player and starts
// falling. The player is expected to fall alongside it (handled by
// Player/collision logic) unless they reach a neighboring platform.
class BreakablePlatform : public Platform
{
public:
    explicit BreakablePlatform(const sf::Texture &texture);

    void update(float deltaTime) override;
    float onPlayerLand() override;

    bool isBroken() const;

private:
    bool broken = false;
};
