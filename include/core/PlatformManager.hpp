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
// Reachability guarantee: every solid (non-Breakable) platform is placed
// no more than BAND_HEIGHT above the previous solid platform's Y
// position (BAND_HEIGHT being at most a single jump's reach). This
// distance is measured directly between actual solid-platform positions
// -- not "somewhere within a fixed screen region" -- so it holds
// regardless of where exactly the previous solid platform ended up.
// Breakable platforms are scattered independently at random and never
// affect this guarantee, since a solid path exists without them.
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
    Platform *createSolidPlatform(float x, float y);
    Platform *createBreakablePlatform(float x, float y);

    // Spawns exactly one solid platform at most BAND_HEIGHT above
    // lastSolidY, then independently rolls for a bonus Breakable
    // platform somewhere in that same vertical range.
    void spawnNextStep();

    std::vector<Platform *> platforms;

    ResourceManager<sf::Texture> &textures;
    unsigned int windowWidth;
    unsigned int windowHeight;

    // Y position of the most recently spawned SOLID platform. The next
    // solid platform is always placed within BAND_HEIGHT of this value
    // (measured from this exact point, not from a fixed grid line), so
    // consecutive solid platforms are always a single reachable jump
    // apart no matter where each one lands.
    float lastSolidY = 0.f;
    float lastSolidX = 0.f; // X position of that same platform, used to avoid overlap with bonus Breakables

    std::mt19937 rng{std::random_device{}()};
    std::bernoulli_distribution movingVsNormal;
    std::bernoulli_distribution breakableBonusRoll;
    std::bernoulli_distribution springDistribution;
};
