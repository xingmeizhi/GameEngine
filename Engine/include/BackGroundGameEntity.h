#pragma once
#include "GameEntity.h"

/*!
 * \struct BackGroundGameEntity
 * \brief The BackGroundGameEntity struct is a specialized game entity for displaying a background image.
 *
 * Inherits from GameEntity and adds a SpriteComponent to render a static background image.
 */
struct BackGroundGameEntity : public GameEntity
{
    BackGroundGameEntity(SDL_Renderer *renderer)
    {
        this->AddComponent<SpriteComponent>(renderer, "assets/background.bmp")->SetSize(640, 480);
    }

    virtual ~BackGroundGameEntity()
    {
    }

    virtual void Input(float deltaTime) override
    {
    }

    virtual void Update(float deltaTime) override
    {
        auto BackGroundGameEntity = this->GetComponent<SpriteComponent>();
        if (!BackGroundGameEntity)
        {
            return;
        }
    }

    virtual void Render(SDL_Renderer *renderer) override
    {
        if (!mRenderable)
        {
            return;
        }

        auto spriteComponent = this->GetComponent<SpriteComponent>();
        if (spriteComponent)
        {
            spriteComponent->Render(renderer);
        }
    }

private:
};