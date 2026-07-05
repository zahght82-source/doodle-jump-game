#include "entities/platforms/Spring.hpp"
#include "utils/Constants.hpp"
#include <algorithm>

Spring::Spring(const sf::Texture &texture)
    : GameObject(texture)
{
}

void Spring::update(float deltaTime)
{
    if (compressTimer > 0.f)
    {
        compressTimer = std::max(0.f, compressTimer - deltaTime);

        // Interpolate the vertical scale back from compressed (squashed
        // flat) to normal (1.0) as the timer runs out, giving a quick
        // squash-and-recover animation without needing a second sprite
        // frame -- the provided spring asset is a single image.
        float t = compressTimer / Constants::SPRING_COMPRESS_DURATION; // 1 -> 0
        float scaleY = Constants::SPRING_COMPRESSED_SCALE_Y + (1.f - Constants::SPRING_COMPRESSED_SCALE_Y) * (1.f - t);
        sprite.setScale({1.f, scaleY});
    }
    else
    {
        sprite.setScale({1.f, 1.f});
    }
}

void Spring::attachTo(sf::Vector2f platformPosition, sf::Vector2f platformSize)
{
    setPosition({platformPosition.x,
                  platformPosition.y - platformSize.y / 2.f - Constants::SPRING_HEIGHT / 2.f});
}

void Spring::compress()
{
    compressTimer = Constants::SPRING_COMPRESS_DURATION;
}
