#include "entities/platforms/Platform.hpp"

Platform::Platform(const sf::Texture &texture)
    : GameObject(texture)
{
}

bool Platform::isSolid() const
{
    return solid;
}

bool Platform::isExpired() const
{
    return expired;
}
