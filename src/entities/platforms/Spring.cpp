#include "entities/platforms/Spring.hpp"
#include "utils/Constants.hpp"

Spring::Spring(const sf::Texture &texture)
    : GameObject(texture)
{
}

void Spring::update(float /*deltaTime*/)
{
    // Springs are static decorations; they don't move on their own.
    // (Present so Spring is a fully concrete GameObject, satisfying the
    // pure virtual update() contract.)
}

void Spring::attachTo(sf::Vector2f platformPosition, sf::Vector2f platformSize)
{
    setPosition({platformPosition.x,
                  platformPosition.y - platformSize.y / 2.f - Constants::SPRING_HEIGHT / 2.f});
}
