#pragma once
#include "GameEntity.h"

/*!
 * \struct PlayerGameEntity
 * \brief The PlayerGameEntity struct is specialized to represent the player character in the game.
 *
 * Inherits from GameEntity and adds a SpriteComponent for the player's visual representation. This entity handles
 * user input for movements like walking and jumping, and manages the physics of those actions.
 */
struct PlayerGameEntity : public GameEntity
{
    PlayerGameEntity(SDL_Renderer *renderer) : GameEntity()
    {
        AddComponent<SpriteComponent>(renderer, "assets/hero.bmp");
    }

    virtual ~PlayerGameEntity()
    {
    }

    void Input(float deltaTime) override
    {
        const Uint8 *state = SDL_GetKeyboardState(nullptr);
        auto spriteComponent = this->GetComponent<SpriteComponent>();
        if (!spriteComponent)
        {
            return;
        }

        float newX = spriteComponent->GetX();
        float newY = spriteComponent->GetY();

        if (state[SDL_SCANCODE_LEFT])
        {
            newX -= mSpeed * deltaTime;
        }
        else if (state[SDL_SCANCODE_RIGHT])
        {
            newX += mSpeed * deltaTime;
        }

        if (state[SDL_SCANCODE_SPACE] && mIsOnGround)
        {
            mVerticalSpeed = -mJumpSpeed;
            mIsOnGround = false;
        }
        spriteComponent->SetX(std::clamp(newX, 0.0f, 640.0f - spriteComponent->GetWidth()));
    }

    virtual void Update(float deltaTime) override
    {
        auto spriteComponent = this->GetComponent<SpriteComponent>();
        if (!spriteComponent)
        {
            return;
        }

        if (!mIsOnGround)
        {
            mVerticalSpeed += mGravity * deltaTime;
            float newY = spriteComponent->GetY() + mVerticalSpeed * deltaTime;
            if (newY >= 480.0f - spriteComponent->GetHeight())
            {
                newY = 480.0f - spriteComponent->GetHeight();
                mIsOnGround = true;
                mVerticalSpeed = 0.0f;
            }

            spriteComponent->SetY(newY);
        }
    }

    /*!
     * \brief Renders the player entity using the associated renderer.
     * \param renderer The SDL renderer to draw the player sprite.
     *
     * Renders the player sprite if the entity is marked as renderable. Retrieves the SpriteComponent
     * and invokes its render method.
     */
    void Render(SDL_Renderer *renderer) override
    {
        auto spriteComponent = this->GetComponent<SpriteComponent>();
        if (spriteComponent)
        {
            spriteComponent->Render(renderer);
        }
    }

    /*!
     * \brief Sets the player's ground state.
     * \param newY The new Y-coordinate for players at the ground.
     *
     * This method is called to set the player's position to the ground level and adjust the jumping state accordingly.
     */
    void SetOnGround(float newY)
    {
        auto spriteComponent = GetComponent<SpriteComponent>();

        if (spriteComponent)
        {
            spriteComponent->SetY(newY);
            mVerticalSpeed = 0.0f;
            mIsOnGround = true;
        }
    }

    /*!
     * \brief Controls whether the player should start falling.
     * \param value True if the player should fall, false if otherwise.
     *
     * Sets the player's on-ground state to false to initiate falling, typically used after jumping.
     */
    void SetShouldFall(bool value)
    {
        if (value)
        {
            mIsOnGround = false;
        }
    }

    /*!
     * \brief Checks if the player is currently jumping.
     * \return True if the player is in the air, false if on the ground.
     */
    bool IsJumping() const
    {
        return mVerticalSpeed != 0.0f;
    }

    /*!
     * \brief Gets the player's current vertical speed.
     * \return The current vertical speed of the player.
     *
     * This method returns the player's vertical speed.
     */
    float GetVerticalSpeed() const
    {
        return mVerticalSpeed;
    }

private:
    float mSpeed{150.0f};
    float mJumpSpeed{450.0f};
    float mGravity{980.0f};
    float mVerticalSpeed{0.0f};
    bool mIsOnGround{true};
};
