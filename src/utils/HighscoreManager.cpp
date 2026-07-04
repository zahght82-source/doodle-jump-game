#include "utils/HighscoreManager.hpp"
#include <fstream>

HighscoreManager::HighscoreManager(const std::string &filePath)
    : filePath(filePath)
{
    load();
}

void HighscoreManager::load()
{
    std::ifstream in(filePath);
    if (in.is_open())
    {
        in >> highscore;
    }
    // If the file doesn't exist yet, highscore stays at its default (0).
}

void HighscoreManager::save() const
{
    std::ofstream out(filePath, std::ios::trunc);
    if (out.is_open())
    {
        out << highscore;
    }
}

int HighscoreManager::getHighscore() const
{
    return highscore;
}

bool HighscoreManager::submitScore(int score)
{
    if (score > highscore)
    {
        highscore = score;
        save();
        return true;
    }
    return false;
}
