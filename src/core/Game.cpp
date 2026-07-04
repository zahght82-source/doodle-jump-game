#include "core/Game.hpp"
#include "core/CollisionHandler.hpp"
#include "utils/Constants.hpp"
#include <sstream>
#include <algorithm>

Game::Game()
    : window(sf::VideoMode({Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT}), Constants::WINDOW_TITLE),
      gameView(sf::FloatRect({0.f, 0.f},
                              {static_cast<float>(Constants::WINDOW_WIDTH), static_cast<float>(Constants::WINDOW_HEIGHT)})),
      highscoreManager(Constants::Paths::HIGHSCORE_FILE)
{
    window.setFramerateLimit(Constants::FRAME_RATE_LIMIT);

    // --- Background sprite, scaled to always cover the window ---
    sf::Texture &bgTexture = textureManager.get(Constants::Paths::ASSETS_DIR, Constants::Assets::BACKGROUND);
    backgroundSprite = std::make_unique<sf::Sprite>(bgTexture);
    sf::Vector2u bgSize = bgTexture.getSize();
    float scaleX = static_cast<float>(Constants::WINDOW_WIDTH) / static_cast<float>(bgSize.x);
    float scaleY = static_cast<float>(Constants::WINDOW_HEIGHT) / static_cast<float>(bgSize.y);
    float bgScale = std::max(scaleX, scaleY); // cover, not stretch-distort
    backgroundSprite->setScale({bgScale, bgScale});

    // --- Menu button ---
    startButton = std::make_unique<Button>(
        textureManager.get(Constants::Paths::ASSETS_DIR, Constants::Assets::START_BUTTON));
    startButton->setScale({Constants::BUTTON_SCALE, Constants::BUTTON_SCALE});
    startButton->setPosition({static_cast<float>(Constants::WINDOW_WIDTH) / 2.f,
                               static_cast<float>(Constants::WINDOW_HEIGHT) * 0.55f});

    // --- Game-over buttons ---
    // Laid out top-to-bottom with fixed pixel spacing (title -> score ->
    // restart button -> flavor text -> menu button) so nothing overlaps
    // regardless of window size.
    gameOverRestartButton = std::make_unique<Button>(
        textureManager.get(Constants::Paths::ASSETS_DIR, Constants::Assets::RESTART_BUTTON));
    gameOverRestartButton->setScale({Constants::BUTTON_SCALE, Constants::BUTTON_SCALE});
    gameOverRestartButton->setPosition({static_cast<float>(Constants::WINDOW_WIDTH) / 2.f, 400.f});

    gameOverMenuButton = std::make_unique<Button>(
        textureManager.get(Constants::Paths::ASSETS_DIR, Constants::Assets::MENU_BUTTON));
    gameOverMenuButton->setScale({Constants::BUTTON_SCALE, Constants::BUTTON_SCALE});
    gameOverMenuButton->setPosition({static_cast<float>(Constants::WINDOW_WIDTH) / 2.f, 540.f});
}

void Game::run()
{
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents()
{
    while (const std::optional<sf::Event> event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }

        if (state == GameState::Playing)
        {
            if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::Left || keyPressed->code == sf::Keyboard::Key::A)
                {
                    player->moveLeft();
                }
                else if (keyPressed->code == sf::Keyboard::Key::Right || keyPressed->code == sf::Keyboard::Key::D)
                {
                    player->moveRight();
                }
            }
            else if (const auto *keyReleased = event->getIf<sf::Event::KeyReleased>())
            {
                if (keyReleased->code == sf::Keyboard::Key::Left || keyReleased->code == sf::Keyboard::Key::A ||
                    keyReleased->code == sf::Keyboard::Key::Right || keyReleased->code == sf::Keyboard::Key::D)
                {
                    player->stopHorizontal();
                }
            }
        }
        else if (const auto *mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
        {
            if (mouseReleased->button == sf::Mouse::Button::Left)
            {
                if (state == GameState::Menu)
                {
                    handleMenuClick(mouseReleased->position);
                }
                else if (state == GameState::GameOver)
                {
                    handleGameOverClick(mouseReleased->position);
                }
            }
        }
        else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            // Keyboard shortcuts alongside button clicks: Enter/Space
            // starts from the menu or restarts from game over.
            if (keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::Space)
            {
                if (state == GameState::Menu || state == GameState::GameOver)
                {
                    startNewGame();
                }
            }
        }
    }
}

void Game::handleMenuClick(sf::Vector2i mousePos)
{
    if (startButton->isHovered(mousePos))
    {
        startNewGame();
    }
}

void Game::handleGameOverClick(sf::Vector2i mousePos)
{
    if (gameOverRestartButton->isHovered(mousePos))
    {
        startNewGame();
    }
    else if (gameOverMenuButton->isHovered(mousePos))
    {
        returnToMenu();
    }
}

void Game::update(float deltaTime)
{
    switch (state)
    {
    case GameState::Menu:
        updateMenu();
        break;
    case GameState::Playing:
        updatePlaying(deltaTime);
        break;
    case GameState::GameOver:
        updateGameOver();
        break;
    }
}

void Game::updateMenu()
{
    // Static screen; all interaction is handled in processEvents().
}

void Game::updatePlaying(float deltaTime)
{
    player->update(deltaTime);
    platformManager->update(deltaTime);

    CollisionHandler::resolvePlatformCollisions(*player, *platformManager);

    // Scroll the view up (never back down) once the player crosses the
    // trigger line near the top of the current view. We move the view
    // instead of every object in the world. The view's center only ever
    // decreases (moves up), matching the "never scroll back down" rule.
    float triggerY = gameView.getCenter().y - static_cast<float>(Constants::WINDOW_HEIGHT) * Constants::SCROLL_TRIGGER_RATIO;
    if (player->getPosition().y < triggerY)
    {
        float newCenterY = player->getPosition().y + static_cast<float>(Constants::WINDOW_HEIGHT) * Constants::SCROLL_TRIGGER_RATIO;
        gameView.setCenter({gameView.getCenter().x, newCenterY});
    }
    viewTop = gameView.getCenter().y - static_cast<float>(Constants::WINDOW_HEIGHT) / 2.f;

    platformManager->refresh(viewTop);

    // Score is how many pixels of height the player has climbed above
    // their starting point (higher climb = smaller y = bigger score).
    float startY = static_cast<float>(Constants::WINDOW_HEIGHT) * Constants::PLAYER_START_Y_RATIO;
    float climbed = startY - player->getMaxHeightReached();
    score = std::max(0, static_cast<int>(climbed * Constants::SCORE_PER_UNIT_HEIGHT));

    // Game over once the player falls below the bottom of the view.
    float viewBottom = gameView.getCenter().y + static_cast<float>(Constants::WINDOW_HEIGHT) / 2.f;
    if (player->getPosition().y - player->getBounds().size.y / 2.f > viewBottom)
    {
        newHighscore = highscoreManager.submitScore(score);
        state = GameState::GameOver;
    }
}

void Game::updateGameOver()
{
    // Static screen; restart/menu handled in processEvents().
}

void Game::startNewGame()
{
    player = std::make_unique<Player>(
        textureManager.get(Constants::Paths::ASSETS_DIR, Constants::Assets::LEFT_DOODLE),
        textureManager.get(Constants::Paths::ASSETS_DIR, Constants::Assets::RIGHT_DOODLE));
    player->setPosition({static_cast<float>(Constants::WINDOW_WIDTH) / 2.f,
                          static_cast<float>(Constants::WINDOW_HEIGHT) * Constants::PLAYER_START_Y_RATIO});

    platformManager = std::make_unique<PlatformManager>(textureManager, Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);

    gameView.setCenter({static_cast<float>(Constants::WINDOW_WIDTH) / 2.f, static_cast<float>(Constants::WINDOW_HEIGHT) / 2.f});
    viewTop = 0.f;

    score = 0;
    newHighscore = false;
    state = GameState::Playing;
}

void Game::returnToMenu()
{
    player.reset();
    platformManager.reset();
    state = GameState::Menu;
}

void Game::render()
{
    window.clear(Constants::Colors::BACKGROUND_FALLBACK);

    switch (state)
    {
    case GameState::Menu:
        renderMenu();
        break;
    case GameState::Playing:
        renderPlaying();
        break;
    case GameState::GameOver:
        renderGameOver();
        break;
    }

    window.display();
}

void Game::renderBackground()
{
    window.setView(window.getDefaultView());
    window.draw(*backgroundSprite);
}

void Game::renderMenu()
{
    renderBackground();

    sf::Font &font = fontManager.get(Constants::Paths::FONTS_DIR, Constants::Assets::FONT);

    sf::Text title(font, Constants::GAME_TITLE, 52);
    title.setFillColor(Constants::Colors::TEXT_PRIMARY);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin({titleBounds.position.x + titleBounds.size.x / 2.f,
                      titleBounds.position.y + titleBounds.size.y / 2.f});
    title.setPosition({static_cast<float>(Constants::WINDOW_WIDTH) / 2.f, 220.f});
    window.draw(title);

    std::ostringstream oss;
    oss << "Best: " << highscoreManager.getHighscore();
    sf::Text highscoreText(font, oss.str(), 24);
    highscoreText.setFillColor(Constants::Colors::TEXT_PRIMARY);
    sf::FloatRect hsBounds = highscoreText.getLocalBounds();
    highscoreText.setOrigin({hsBounds.position.x + hsBounds.size.x / 2.f,
                              hsBounds.position.y + hsBounds.size.y / 2.f});
    highscoreText.setPosition({static_cast<float>(Constants::WINDOW_WIDTH) / 2.f, 300.f});
    window.draw(highscoreText);

    startButton->render(window);
}

void Game::renderPlaying()
{
    renderBackground();

    window.setView(gameView);
    platformManager->render(window);
    player->render(window);

    // Score HUD is drawn in the default (non-scrolling) view.
    window.setView(window.getDefaultView());
    sf::Font &font = fontManager.get(Constants::Paths::FONTS_DIR, Constants::Assets::FONT);
    std::ostringstream oss;
    oss << "Score: " << score;
    sf::Text scoreText(font, oss.str(), 22);
    scoreText.setFillColor(Constants::Colors::TEXT_PRIMARY);
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setPosition({10.f, 10.f});
    window.draw(scoreText);
}

void Game::renderGameOver()
{
    renderBackground();

    sf::Font &font = fontManager.get(Constants::Paths::FONTS_DIR, Constants::Assets::FONT);

    sf::Text title(font, "Game Over", 44);
    title.setFillColor(Constants::Colors::TEXT_GAME_OVER);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin({titleBounds.position.x + titleBounds.size.x / 2.f,
                      titleBounds.position.y + titleBounds.size.y / 2.f});
    title.setPosition({static_cast<float>(Constants::WINDOW_WIDTH) / 2.f, 160.f});
    window.draw(title);

    std::ostringstream oss;
    oss << "Score: " << score << "   Best: " << highscoreManager.getHighscore();
    if (newHighscore)
    {
        oss << " (NEW!)";
    }
    sf::Text scoreText(font, oss.str(), 20);
    scoreText.setFillColor(Constants::Colors::TEXT_PRIMARY);
    sf::FloatRect scoreBounds = scoreText.getLocalBounds();
    scoreText.setOrigin({scoreBounds.position.x + scoreBounds.size.x / 2.f,
                          scoreBounds.position.y + scoreBounds.size.y / 2.f});
    scoreText.setPosition({static_cast<float>(Constants::WINDOW_WIDTH) / 2.f, 240.f});
    window.draw(scoreText);

    gameOverRestartButton->render(window);

    sf::Text flavorText(font, Constants::RESTART_FLAVOR_TEXT, 14);
    flavorText.setFillColor(Constants::Colors::TEXT_PRIMARY);
    flavorText.setStyle(sf::Text::Italic);
    sf::FloatRect flavorBounds = flavorText.getLocalBounds();
    flavorText.setOrigin({flavorBounds.position.x + flavorBounds.size.x / 2.f,
                           flavorBounds.position.y + flavorBounds.size.y / 2.f});
    flavorText.setPosition({static_cast<float>(Constants::WINDOW_WIDTH) / 2.f, 460.f});
    window.draw(flavorText);

    gameOverMenuButton->render(window);
}
