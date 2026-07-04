#pragma once
#include <string>

// Loads and saves the single best score to disk.
class HighscoreManager
{
public:
    explicit HighscoreManager(const std::string &filePath);

    int getHighscore() const;

    // Updates and persists the highscore if `score` beats the current one.
    // Returns true if a new record was set.
    bool submitScore(int score);

private:
    void load();
    void save() const;

    std::string filePath;
    int highscore = 0;
};
