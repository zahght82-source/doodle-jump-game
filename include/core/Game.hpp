#pragma once
#include <SFML/Graphics.hpp>
#include "core/ResourceManager.hpp"
#include "core/PlatformManager.hpp"
#include "entities/Player.hpp"
#include "states/GameState.hpp"
#include "utils/HighscoreManager.hpp"
#include "ui/Button.hpp"
#include <memory>

// Owns the window, the current game state, and every game-world object.
// Runs the main loop: poll events -> update -> render.
class Game
{
public:
    Game();

    void run();

private:
    void processEvents();
    void handleMenuClick(sf::Vector2i mousePos);
    void handleGameOverClick(sf::Vector2i mousePos);

    void update(float deltaTime);
    void render();

    void startNewGame();
    void returnToMenu();
    void updateMenu();
    void updatePlaying(float deltaTime);
    void updateGameOver();

    void renderMenu();
    void renderPlaying();
    void renderGameOver();
    void renderBackground();

    sf::RenderWindow window;
    sf::View gameView;
    float viewTop = 0.f; // tracks the highest the view has scrolled to (never moves back down)

    ResourceManager<sf::Texture> textureManager;
    ResourceManager<sf::Font> fontManager;
    HighscoreManager highscoreManager;

    GameState state = GameState::Menu;

    std::unique_ptr<Player> player;
    std::unique_ptr<PlatformManager> platformManager;
    std::unique_ptr<sf::Sprite> backgroundSprite;

    std::unique_ptr<Button> startButton;
    std::unique_ptr<Button> gameOverRestartButton;
    std::unique_ptr<Button> gameOverMenuButton;

    int score = 0;
    bool newHighscore = false;

    sf::Clock clock;
};
