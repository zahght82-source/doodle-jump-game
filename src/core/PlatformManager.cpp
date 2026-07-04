#include "core/PlatformManager.hpp"
#include "entities/platforms/NormalPlatform.hpp"
#include "entities/platforms/MovingPlatform.hpp"
#include "entities/platforms/BreakablePlatform.hpp"
#include "utils/Constants.hpp"
#include <algorithm>

PlatformManager::PlatformManager(ResourceManager<sf::Texture> &textures, unsigned int windowWidth, unsigned int windowHeight)
    : textures(textures),
      windowWidth(windowWidth),
      windowHeight(windowHeight),
      typeDistribution({Constants::WEIGHT_NORMAL_PLATFORM,
                         Constants::WEIGHT_MOVING_PLATFORM,
                         Constants::WEIGHT_BREAKABLE_PLATFORM}),
      gapDistribution(Constants::PLATFORM_MIN_GAP_Y, Constants::PLATFORM_MAX_GAP_Y),
      springDistribution(Constants::SPRING_SPAWN_CHANCE)
{
    std::uniform_real_distribution<float> xDist(
        Constants::PLATFORM_WIDTH / 2.f,
        static_cast<float>(windowWidth) - Constants::PLATFORM_WIDTH / 2.f);

    // The very first platform is guaranteed to be a plain, solid
    // NormalPlatform (no spring) directly beneath the player's start
    // position, so the player never spawns with nothing to land on.
    float startX = static_cast<float>(windowWidth) / 2.f;
    float y = static_cast<float>(windowHeight) * Constants::PLAYER_START_Y_RATIO + Constants::PLAYER_HEIGHT;
    Platform *firstPlatform = new NormalPlatform(
        textures.get(Constants::Paths::ASSETS_DIR, Constants::Assets::NORMAL_PLATFORM));
    firstPlatform->setPosition({startX, y});
    platforms.push_back(firstPlatform);
    y -= gapDistribution(rng);

    // Remaining initial platforms are randomized as usual.
    for (int i = 1; i < Constants::INITIAL_PLATFORM_COUNT; ++i)
    {
        float x = xDist(rng);
        platforms.push_back(spawnPlatform(x, y));
        y -= gapDistribution(rng);
    }
}

PlatformManager::~PlatformManager()
{
    for (Platform *platform : platforms)
    {
        delete platform;
    }
}

PlatformManager::PlatformType PlatformManager::pickRandomType()
{
    int index = typeDistribution(rng);
    return static_cast<PlatformType>(index);
}

Platform *PlatformManager::spawnPlatform(float x, float y)
{
    PlatformType type = pickRandomType();
    Platform *platform = nullptr;

    switch (type)
    {
    case PlatformType::Normal:
    {
        bool withSpring = springDistribution(rng);
        const sf::Texture *springTex = withSpring
            ? &textures.get(Constants::Paths::ASSETS_DIR, Constants::Assets::SPRING_SPRITE)
            : nullptr;
        platform = new NormalPlatform(
            textures.get(Constants::Paths::ASSETS_DIR, Constants::Assets::NORMAL_PLATFORM),
            springTex);
        break;
    }
    case PlatformType::Moving:
        platform = new MovingPlatform(
            textures.get(Constants::Paths::ASSETS_DIR, Constants::Assets::MOVING_PLATFORM),
            windowWidth);
        break;
    case PlatformType::Breakable:
        platform = new BreakablePlatform(
            textures.get(Constants::Paths::ASSETS_DIR, Constants::Assets::BROKEN_PLATFORM));
        break;
    }

    platform->setPosition({x, y});
    return platform;
}

float PlatformManager::highestPlatformY() const
{
    float minY = static_cast<float>(windowHeight);
    for (const Platform *platform : platforms)
    {
        minY = std::min(minY, platform->getPosition().y);
    }
    return minY;
}

void PlatformManager::update(float deltaTime)
{
    for (Platform *platform : platforms)
    {
        platform->update(deltaTime); // virtual dispatch: each type moves/behaves differently
    }
}

void PlatformManager::render(sf::RenderWindow &window)
{
    for (Platform *platform : platforms)
    {
        platform->render(window);
    }
}

void PlatformManager::refresh(float viewTop)
{
    // Remove platforms that fell too far below the view or that are
    // expired (e.g. a BreakablePlatform that fell off-screen).
    float despawnY = viewTop + static_cast<float>(windowHeight) + Constants::PLATFORM_HEIGHT;

    for (auto it = platforms.begin(); it != platforms.end();)
    {
        Platform *platform = *it;
        bool tooLow = platform->getPosition().y > despawnY;
        if (tooLow || platform->isExpired())
        {
            delete platform;
            it = platforms.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Spawn new platforms above the current highest one until we have
    // enough to fill the visible area plus a buffer above it.
    float targetTop = viewTop - static_cast<float>(windowHeight) * 0.5f;
    std::uniform_real_distribution<float> xDist(
        Constants::PLATFORM_WIDTH / 2.f,
        static_cast<float>(windowWidth) - Constants::PLATFORM_WIDTH / 2.f);

    while (highestPlatformY() > targetTop)
    {
        float newY = highestPlatformY() - gapDistribution(rng);
        float newX = xDist(rng);
        platforms.push_back(spawnPlatform(newX, newY));
    }
}

const std::vector<Platform *> &PlatformManager::getPlatforms() const
{
    return platforms;
}
