#pragma once
#include <SFML/Graphics.hpp>

// All magic numbers used across the game live here.
namespace Constants
{
    // ---- Window ----
    constexpr unsigned int WINDOW_WIDTH = 480;
    constexpr unsigned int WINDOW_HEIGHT = 720;
    constexpr const char *WINDOW_TITLE = "Doodle Jump";
    constexpr unsigned int FRAME_RATE_LIMIT = 60;

    // ---- Physics ----
    constexpr float GRAVITY = 900.f;           // px/s^2, downward acceleration
    constexpr float JUMP_VELOCITY = -650.f;    // px/s, initial upward velocity on normal jump
    constexpr float SPRING_JUMP_VELOCITY = -1000.f; // px/s, boosted jump velocity from a spring
    constexpr float PLAYER_MOVE_SPEED = 300.f; // px/s, horizontal speed
    // Max jump height = v^2 / (2*g) = 650^2 / 1800 ~= 235 px.
    // Platform gaps are kept well under this (see PLATFORM_MAX_GAP_Y) so
    // the player can always reach the next platform even accounting for
    // the discrete per-frame integration step.

    // ---- Player ----
    constexpr float PLAYER_WIDTH = 50.f;
    constexpr float PLAYER_HEIGHT = 50.f;
    constexpr float PLAYER_START_Y_RATIO = 0.8f; // fraction of window height for start position

    // ---- Platforms ----
    constexpr float PLATFORM_WIDTH = 70.f;
    constexpr float PLATFORM_HEIGHT = 20.f;
    constexpr float MOVING_PLATFORM_SPEED = 100.f; // px/s horizontal speed
    constexpr float BREAKABLE_FALL_SPEED = 250.f;  // px/s, speed the broken platform falls at

    // Vertical gap between consecutively spawned platforms (must always be
    // reachable given JUMP_VELOCITY and GRAVITY -- see reachability check).
    constexpr float PLATFORM_MIN_GAP_Y = 70.f;
    constexpr float PLATFORM_MAX_GAP_Y = 160.f;

    // How many platforms to keep alive in the world at once.
    constexpr int INITIAL_PLATFORM_COUNT = 8;

    // ---- Spawn probabilities (weights, not required to sum to 1) ----
    constexpr double WEIGHT_NORMAL_PLATFORM = 0.60;
    constexpr double WEIGHT_MOVING_PLATFORM = 0.20;
    constexpr double WEIGHT_BREAKABLE_PLATFORM = 0.20;
    constexpr double SPRING_SPAWN_CHANCE = 0.15; // chance a Normal platform also gets a spring

    // ---- Spring ----
    constexpr float SPRING_WIDTH = 24.f;
    constexpr float SPRING_HEIGHT = 24.f;

    // ---- Scroll / View ----
    constexpr float SCROLL_TRIGGER_RATIO = 0.5f; // once player crosses top half, view scrolls

    // ---- Score ----
    constexpr float SCORE_PER_UNIT_HEIGHT = 0.1f; // score gained per pixel of height climbed

    // ---- UI ----
    constexpr const char *GAME_TITLE = "Doodle Jump";
    constexpr const char *RESTART_FLAVOR_TEXT = "Try not to fall this time, genius!";
    constexpr float BUTTON_SCALE = 0.5f;

    // ---- Asset file names (relative to assets/ and fonts/) ----
    namespace Assets
    {
        constexpr const char *BACKGROUND = "background.png";
        constexpr const char *NORMAL_PLATFORM = "normal_platform.png";
        constexpr const char *MOVING_PLATFORM = "moving_platform.png";
        constexpr const char *BROKEN_PLATFORM = "broken_platform.png";
        constexpr const char *SPRING_SPRITE = "spring_sprite.png";
        constexpr const char *LEFT_DOODLE = "left_doodle.png";
        constexpr const char *RIGHT_DOODLE = "right_doodle.png";
        constexpr const char *START_BUTTON = "start_button.png";
        constexpr const char *MENU_BUTTON = "menu_button.png";
        constexpr const char *RESTART_BUTTON = "restart_button.png";
        constexpr const char *FONT = "ariblk.ttf";
    }

    // ---- Paths ----
    namespace Paths
    {
        constexpr const char *ASSETS_DIR = "assets/";
        constexpr const char *FONTS_DIR = "fonts/";
        constexpr const char *HIGHSCORE_FILE = "highscore.txt";
    }

    // ---- Colors ----
    namespace Colors
    {
        const sf::Color BACKGROUND_FALLBACK = sf::Color(135, 206, 235);
        const sf::Color TEXT_PRIMARY = sf::Color::Black;
        const sf::Color TEXT_GAME_OVER = sf::Color(214, 66, 66);
    }
}
