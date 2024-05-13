#pragma once
#include "GameEntity.h"

/*!
 * \struct FoodGameEntity
 * \brief The FoodGameEntity struct is designed to represent food entities in the game, handling their visualization and behavior.
 *
 * Inherits from GameEntity and adds a SpriteComponent to render a food image. This entity is used to represent
 * food items in the game.
 */
struct FoodGameEntity : public GameEntity
{

    FoodGameEntity(SDL_Renderer *renderer)
    {
        this->AddComponent<SpriteComponent>(renderer, "assets/food.bmp")->SetSize(45.0f, 45.0f);
    }

    virtual ~FoodGameEntity()
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