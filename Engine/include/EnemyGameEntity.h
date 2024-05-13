#pragma once
#include "GameEntity.h"

/*!
 * \struct EnemyGameEntity
 * \brief The EnemyGameEntity struct is specialized for enemy entities in the game, handling their visualization and behavior.
 *
 * Inherits from GameEntity and adds a SpriteComponent to render an enemy image. This entity is used to represent
 * enemies in the game.
 */
struct EnemyGameEntity : public GameEntity
{

    EnemyGameEntity(SDL_Renderer *renderer)
    {
        this->AddComponent<SpriteComponent>(renderer, "assets/enemy.bmp")->SetSize(45.0f, 45.0f);
    }

    virtual ~EnemyGameEntity()
    {
    }

    virtual void Input(float deltaTime) override
    {
    }

    virtual void Update(float deltaTime) override
    {
        auto spriteComponent = this->GetComponent<SpriteComponent>();
        if (!spriteComponent)
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
};