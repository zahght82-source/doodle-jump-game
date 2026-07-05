#include "entities/Player.hpp"
#include "utils/Constants.hpp"

Player::Player(const sf::Texture &leftTexture, const sf::Texture &rightTexture)
    : GameObject(rightTexture),
      leftTexture(leftTexture),
      rightTexture(rightTexture)
{
}

void Player::update(float deltaTime)
{
    // Record feet position before this frame's movement, for the
    // crossing-based collision check in CollisionHandler.
    previousBottom = getBounds().position.y + getBounds().size.y;

    // --- Horizontal movement (always allowed, even while dragged down) ---
    velocity.x = horizontalInput * Constants::PLAYER_MOVE_SPEED;

    if (horizontalInput < 0.f)
    {
        switchTexture(FacingDirection::Left);
    }
    else if (horizontalInput > 0.f)
    {
        switchTexture(FacingDirection::Right);
    }

    // --- Vertical movement ---
    // Per spec 5.2: while being dragged down by a broken platform,
    // gravity is suspended and the player instead falls at exactly the
    // platform's fall speed (velocity.y was already set to that value
    // by CollisionHandler via enterDraggedState/setVerticalVelocity).
    if (!isDragged)
    {
        velocity.y += Constants::GRAVITY * deltaTime;
    }

    sf::Vector2f pos = getPosition();
    pos.x += velocity.x * deltaTime;
    pos.y += velocity.y * deltaTime;
    setPosition(pos);

    handleScreenWrap(Constants::WINDOW_WIDTH);

    // Track the highest point reached (smallest y) for scoring.
    float currentY = getPosition().y;
    if (!maxHeightInitialized || currentY < maxHeightReached)
    {
        maxHeightReached = currentY;
        maxHeightInitialized = true;
    }
}

void Player::moveLeft()
{
    horizontalInput = -1.f;
}

void Player::moveRight()
{
    horizontalInput = 1.f;
}

void Player::stopHorizontal()
{
    horizontalInput = 0.f;
}

void Player::setVerticalVelocity(float velocityY)
{
    velocity.y = velocityY;
}

void Player::enterDraggedState(float fallSpeed)
{
    isDragged = true;
    velocity.y = fallSpeed;
}

void Player::leaveDraggedState()
{
    isDragged = false;
}

bool Player::isBeingDragged() const
{
    return isDragged;
}

bool Player::isFalling() const
{
    return velocity.y > 0.f;
}

float Player::getVerticalVelocity() const
{
    return velocity.y;
}

float Player::getPreviousBottom() const
{
    return previousBottom;
}

float Player::getMaxHeightReached() const
{
    return maxHeightReached;
}

void Player::handleScreenWrap(unsigned int windowWidth)
{
    sf::Vector2f pos = getPosition();
    float halfWidth = getBounds().size.x / 2.f;

    if (pos.x + halfWidth < 0.f)
    {
        pos.x = static_cast<float>(windowWidth) + halfWidth;
        setPosition(pos);
    }
    else if (pos.x - halfWidth > static_cast<float>(windowWidth))
    {
        pos.x = -halfWidth;
        setPosition(pos);
    }
}

void Player::switchTexture(FacingDirection direction)
{
    if (direction == facing)
    {
        return;
    }
    facing = direction;
    sprite.setTexture(direction == FacingDirection::Left ? leftTexture : rightTexture);
}
