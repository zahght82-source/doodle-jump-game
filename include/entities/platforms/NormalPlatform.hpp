#pragma once
#include "entities/platforms/Platform.hpp"
#include "entities/platforms/Spring.hpp"
#include <memory>

// A stable, stationary platform. May optionally own a Spring
// (composition): landing on the spring gives a much stronger jump
// than landing on the bare platform.
class NormalPlatform : public Platform
{
public:
    NormalPlatform(const sf::Texture &texture, const sf::Texture *springTexture = nullptr);

    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

    float onPlayerLand() override;

    bool hasSpring() const;
    sf::FloatRect getSpringBounds() const;

    // Called by the collision system specifically when the player lands
    // on the spring rather than the plain platform surface.
    float onSpringLand();

private:
    std::unique_ptr<Spring> spring; // null if this platform has no spring
};
