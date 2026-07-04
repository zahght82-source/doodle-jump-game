#pragma once
#include "entities/platforms/Platform.hpp"
#include "core/ResourceManager.hpp"
#include <vector>
#include <memory>
#include <random>

// Owns all live Platform* instances. Handles weighted-random spawning of
// platform types, keeps the world stocked with platforms above the
// player, and removes platforms that have scrolled off-screen or expired
// (e.g. a fallen BreakablePlatform).
//
// vector<Platform*> is required here (not vector<Platform>) so that the
// polymorphic Normal/Moving/Breakable objects are stored and dispatched
// through base-class pointers -- storing by value would slice them.
class PlatformManager
{
public:
    PlatformManager(ResourceManager<sf::Texture> &textures, unsigned int windowWidth, unsigned int windowHeight);
    ~PlatformManager();

    // No copying: this class owns raw pointers to heap-allocated platforms.
    PlatformManager(const PlatformManager &) = delete;
    PlatformManager &operator=(const PlatformManager &) = delete;

    void update(float deltaTime);
    void render(sf::RenderWindow &window);

    // Removes any expired/off-screen platforms and spawns new ones above
    // the current highest platform to keep the world populated.
    void refresh(float viewTop);

    const std::vector<Platform *> &getPlatforms() const;

private:
    enum class PlatformType
    {
        Normal,
        Moving,
        Breakable
    };

    Platform *spawnPlatform(float x, float y);
    PlatformType pickRandomType();
    float highestPlatformY() const; // smallest y among current platforms

    std::vector<Platform *> platforms;

    ResourceManager<sf::Texture> &textures;
    unsigned int windowWidth;
    unsigned int windowHeight;

    std::mt19937 rng{std::random_device{}()};
    std::discrete_distribution<int> typeDistribution;
    std::uniform_real_distribution<float> gapDistribution;
    std::bernoulli_distribution springDistribution;
};
