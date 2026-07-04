#pragma once
#include "entities/platforms/Platform.hpp"

// Moves horizontally and bounces off the screen edges.
class MovingPlatform : public Platform
{
public:
    MovingPlatform(const sf::Texture &texture, unsigned int windowWidth);

    void update(float deltaTime) override;
    float onPlayerLand() override;

private:
    float velocityX;
    unsigned int windowWidth;
};
