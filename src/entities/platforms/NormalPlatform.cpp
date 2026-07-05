#include "entities/platforms/NormalPlatform.hpp"
#include "utils/Constants.hpp"

NormalPlatform::NormalPlatform(const sf::Texture &texture, const sf::Texture *springTexture)
    : Platform(texture)
{
    if (springTexture != nullptr)
    {
        spring = std::make_unique<Spring>(*springTexture);
    }
}

void NormalPlatform::update(float deltaTime)
{
    if (spring)
    {
        spring->update(deltaTime);
        spring->attachTo(getPosition(), {Constants::PLATFORM_WIDTH, Constants::PLATFORM_HEIGHT});
    }
}

void NormalPlatform::render(sf::RenderWindow &window)
{
    GameObject::render(window);
    if (spring)
    {
        spring->render(window);
    }
}

float NormalPlatform::onPlayerLand()
{
    return Constants::JUMP_VELOCITY;
}

float NormalPlatform::onSpringLand()
{
    if (spring)
    {
        spring->compress();
    }
    return Constants::SPRING_JUMP_VELOCITY;
}

bool NormalPlatform::hasSpring() const
{
    return spring != nullptr;
}

sf::FloatRect NormalPlatform::getSpringBounds() const
{
    if (!spring)
    {
        return sf::FloatRect(); // empty rect: caller must check hasSpring() first
    }
    return spring->getBounds();
}
