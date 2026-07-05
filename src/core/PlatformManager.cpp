#include "core/PlatformManager.hpp"
#include "entities/platforms/NormalPlatform.hpp"
#include "entities/platforms/MovingPlatform.hpp"
#include "entities/platforms/BreakablePlatform.hpp"
#include "utils/Constants.hpp"
#include <cmath>

PlatformManager::PlatformManager(ResourceManager<sf::Texture> &textures, unsigned int windowWidth, unsigned int windowHeight)
    : textures(textures),
      windowWidth(windowWidth),
      windowHeight(windowHeight),
      movingVsNormal(Constants::MOVING_PLATFORM_CHANCE),
      breakableBonusRoll(Constants::BREAKABLE_BONUS_CHANCE),
      springDistribution(Constants::SPRING_SPAWN_CHANCE)
{
    // The very first platform is guaranteed to be a plain, solid
    // NormalPlatform (no spring) directly beneath the player's start
    // position, so the player never spawns with nothing to land on.
    float startX = static_cast<float>(windowWidth) / 2.f;
    float startY = static_cast<float>(windowHeight) * Constants::PLAYER_START_Y_RATIO + Constants::PLAYER_HEIGHT;
    Platform *firstPlatform = new NormalPlatform(
        textures.get(Constants::Paths::ASSETS_DIR, Constants::Assets::NORMAL_PLATFORM));
    firstPlatform->setPosition({startX, startY});
    platforms.push_back(firstPlatform);

    lastSolidY = startY;
    lastSolidX = startX;

    // Populate enough initial steps above the starting platform.
    for (int i = 0; i < Constants::INITIAL_BAND_COUNT; ++i)
    {
        spawnNextStep();
    }
}

PlatformManager::~PlatformManager()
{
    for (Platform *platform : platforms)
    {
        delete platform;
    }
}

Platform *PlatformManager::createSolidPlatform(float x, float y)
{
    Platform *platform;
    if (movingVsNormal(rng))
    {
        platform = new MovingPlatform(
            textures.get(Constants::Paths::ASSETS_DIR, Constants::Assets::MOVING_PLATFORM),
            windowWidth);
    }
    else
    {
        bool withSpring = springDistribution(rng);
        const sf::Texture *springTex = withSpring
            ? &textures.get(Constants::Paths::ASSETS_DIR, Constants::Assets::SPRING_SPRITE)
            : nullptr;
        platform = new NormalPlatform(
            textures.get(Constants::Paths::ASSETS_DIR, Constants::Assets::NORMAL_PLATFORM),
            springTex);
    }
    platform->setPosition({x, y});
    return platform;
}

Platform *PlatformManager::createBreakablePlatform(float x, float y)
{
    Platform *platform = new BreakablePlatform(
        textures.get(Constants::Paths::ASSETS_DIR, Constants::Assets::BROKEN_PLATFORM));
    platform->setPosition({x, y});
    return platform;
}

void PlatformManager::spawnNextStep()
{
    std::uniform_real_distribution<float> xDist(
        Constants::PLATFORM_WIDTH / 2.f,
        static_cast<float>(windowWidth) - Constants::PLATFORM_WIDTH / 2.f);

    float previousSolidY = lastSolidY;
    float previousSolidX = lastSolidX;

    // The next solid platform is placed somewhere between just above
    // lastSolidY and exactly BAND_HEIGHT above it. Measuring directly
    // from lastSolidY (an actual platform position), rather than from a
    // fixed grid boundary, is what guarantees this exact distance is
    // always within one jump -- regardless of where the previous solid
    // platform happened to land.
    std::uniform_real_distribution<float> solidGapDist(Constants::PLATFORM_HEIGHT * 2.f, Constants::BAND_HEIGHT);
    float newSolidX = xDist(rng);
    float newSolidY = previousSolidY - solidGapDist(rng);
    platforms.push_back(createSolidPlatform(newSolidX, newSolidY));
    lastSolidY = newSolidY;
    lastSolidX = newSolidX;

    // Independent bonus: a Breakable may also spawn strictly between the
    // previous and new solid platform (with padding so it never
    // vertically overlaps either one), purely for variety. Since the
    // solid-to-solid guarantee above doesn't depend on it, this can
    // never block progress no matter where it lands or what happens to
    // it. No two platforms should ever overlap or intersect, so its X is
    // also kept clear of both solid platforms' horizontal extents when
    // they'd otherwise collide at the same height band.
    if (breakableBonusRoll(rng))
    {
        float safeTop = newSolidY + Constants::PLATFORM_HEIGHT * 1.5f;
        float safeBottom = previousSolidY - Constants::PLATFORM_HEIGHT * 1.5f;

        // Only place it if there's actually room between the two solid
        // platforms after applying the padding above.
        if (safeTop < safeBottom)
        {
            std::uniform_real_distribution<float> breakableYDist(safeTop, safeBottom);
            float breakableY = breakableYDist(rng);

            // Pick an X far enough from both solid platforms' X to avoid
            // any horizontal overlap, retrying a few times before giving
            // up (in the rare case the screen is too narrow to avoid it,
            // skipping the bonus platform is preferable to overlapping).
            const int maxAttempts = 5;
            for (int attempt = 0; attempt < maxAttempts; ++attempt)
            {
                float candidateX = xDist(rng);
                float minSeparation = Constants::PLATFORM_WIDTH * 1.1f;
                bool clearOfNew = std::abs(candidateX - newSolidX) >= minSeparation;
                bool clearOfPrevious = std::abs(candidateX - previousSolidX) >= minSeparation;
                if (clearOfNew && clearOfPrevious)
                {
                    platforms.push_back(createBreakablePlatform(candidateX, breakableY));
                    break;
                }
            }
        }
    }
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

    // Keep spawning steps above the last solid platform until we have
    // enough height covered above the current view.
    float targetTop = viewTop - static_cast<float>(windowHeight) * 0.5f;
    while (lastSolidY > targetTop)
    {
        spawnNextStep();
    }
}

const std::vector<Platform *> &PlatformManager::getPlatforms() const
{
    return platforms;
}
