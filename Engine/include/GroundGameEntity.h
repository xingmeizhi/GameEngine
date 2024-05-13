#pragma once
#include "GameEntity.h"

/*!
 * \struct GroundGameEntity
 * \brief The GroundGameEntity struct is designed to represent ground entities in the game, handling their visualization and behavior.
 *
 * Inherits from GameEntity and adds a SpriteComponent to render a ground image. This entity is used to represent
 * the grounds in the game.
 */
struct GroundGameEntity : public GameEntity
{

    GroundGameEntity(SDL_Renderer *renderer, float width, float height)
    {
        this->AddComponent<SpriteComponent>(renderer, "assets/ground.bmp")->SetSize(width, height);
    }

    virtual ~GroundGameEntity()
    {
    }

    virtual void Input(float deltaTime) override
    {
    }

    virtual void Update(float deltaTime) override
    {
        auto GroundGameEntity = this->GetComponent<SpriteComponent>();
        if (!GroundGameEntity)
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