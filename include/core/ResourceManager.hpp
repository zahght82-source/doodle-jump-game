#pragma once
#include <SFML/Graphics/Font.hpp>
#include <map>
#include <memory>
#include <string>
#include <stdexcept>

// SFML resource types don't agree on a load-from-disk method name
// (sf::Texture::loadFromFile vs sf::Font::openFromFile), so this trait
// maps each type T to the correct call. Add a new specialization here
// if a future resource type (e.g. sf::SoundBuffer) needs one.
template <typename T>
struct ResourceLoader
{
    static bool load(T &resource, const std::string &path)
    {
        return resource.loadFromFile(path);
    }
};

// sf::Font renamed loadFromFile to openFromFile in this SFML version.
template <>
struct ResourceLoader<sf::Font>
{
    static bool load(sf::Font &resource, const std::string &path)
    {
        return resource.openFromFile(path);
    }
};

// Generic resource cache. Loads a resource of type T from disk exactly once
// and returns references to the cached copy on subsequent requests.
// Used for sf::Texture and sf::Font (and sf::SoundBuffer in a later phase).
template <typename T>
class ResourceManager
{
public:
    // Loads (if needed) and returns a reference to the resource at `path`.
    // `path` is used both as the cache key and, combined with `baseDir`,
    // as the file location.
    T &get(const std::string &baseDir, const std::string &fileName)
    {
        const std::string key = baseDir + fileName;

        auto it = resources.find(key);
        if (it != resources.end())
        {
            return *(it->second);
        }

        auto resource = std::make_unique<T>();
        if (!ResourceLoader<T>::load(*resource, key))
        {
            throw std::runtime_error("ResourceManager: failed to load resource: " + key);
        }

        T &ref = *resource;
        resources.emplace(key, std::move(resource));
        return ref;
    }

private:
    std::map<std::string, std::unique_ptr<T>> resources;
};
