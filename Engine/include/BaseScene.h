#pragma once
#include "SceneManager.h"
#include "EnemyGameEntity.h"
#include "GroundGameEntity.h"
#include "PlayerGameEntity.h"
#include "BackGroundGameEntity.h"
#include "SpriteComponent.h"
#include "FoodGameEntity.h"

/*!
 * \struct BaseScene
 * \brief The BaseScene struct provides a basic template for game scenes, including player, enemies, food, and grounds.
 *
 * This structure manages a collection of game entities and implements common scene behaviors such as initialization,
 * input handling, updating state, and rendering. It serves as a base for more specific scene implementations in the game.
 */
struct BaseScene : public Scene
{
protected:
    std::vector<std::shared_ptr<EnemyGameEntity>> enemies;
    std::vector<std::shared_ptr<FoodGameEntity>> foods;
    std::unique_ptr<PlayerGameEntity> mainCharacter;
    std::vector<std::shared_ptr<GroundGameEntity>> Grounds;
    std::unique_ptr<BackGroundGameEntity> backGround;
    bool mRun{true};
    float mPoints{0.0f};
    SDL_Window *mWindow = nullptr;
    SDL_Renderer *mRenderer = nullptr;
    bool isCompleted = false;
    bool isWin = false;

public:
    /*!
     * \brief Constructor for BaseScene.
     * \param renderer The SDL renderer for drawing entities.
     * \param window The SDL window for rendering.
     *
     * Initializes the scene with the given renderer and window, setting up the basic scene structure.
     */
    BaseScene(SDL_Renderer *renderer, SDL_Window *window) : mRenderer(renderer), mWindow(window) {}

    /*!
     * \brief Destructor for BaseScene.
     *
     * Handles clean-up and resource deallocation.
     */
    virtual ~BaseScene() override
    {
        Cleanup();
    }

    /*!
     * \brief Initializes the scene.
     *
     * Sets up resources and prepares the scene for starting.
     */
    void Init() override
    {
        StartUp();
    }

    /*!
     * \brief Starts up the scene.
     *
     * Loads and initializes all essential entities for the scene.
     */
    void StartUp() override
    {
        ResourceManager &manager = ResourceManager::GetInstance();
        manager.StartUp();
        mainCharacter = std::make_unique<PlayerGameEntity>(mRenderer);
        backGround = std::make_unique<BackGroundGameEntity>(mRenderer);

        mainCharacter->GetComponent<SpriteComponent>()->Move(220, 460);
        backGround->GetComponent<SpriteComponent>()->Move(0, 0);

        auto ground1 = std::make_unique<GroundGameEntity>(mRenderer, 640, 20);
        auto ground2 = std::make_unique<GroundGameEntity>(mRenderer, 200, 20);
        auto ground3 = std::make_unique<GroundGameEntity>(mRenderer, 200, 20);
        auto ground4 = std::make_unique<GroundGameEntity>(mRenderer, 100, 20);
        auto ground5 = std::make_unique<GroundGameEntity>(mRenderer, 200, 20);
        auto ground6 = std::make_unique<GroundGameEntity>(mRenderer, 300, 20);
        auto ground7 = std::make_unique<GroundGameEntity>(mRenderer, 100, 20);
        auto ground8 = std::make_unique<GroundGameEntity>(mRenderer, 180, 20);

        ground1->GetComponent<SpriteComponent>()->Move(0, 460);
        ground2->GetComponent<SpriteComponent>()->Move(0, 350);
        ground3->GetComponent<SpriteComponent>()->Move(300, 350);
        ground4->GetComponent<SpriteComponent>()->Move(470, 300);
        ground5->GetComponent<SpriteComponent>()->Move(200, 200);
        ground6->GetComponent<SpriteComponent>()->Move(0, 100);
        ground7->GetComponent<SpriteComponent>()->Move(0, 270);
        ground8->GetComponent<SpriteComponent>()->Move(500, 100);

        Grounds.push_back(std::move(ground1));
        Grounds.push_back(std::move(ground2));
        Grounds.push_back(std::move(ground3));
        Grounds.push_back(std::move(ground4));
        Grounds.push_back(std::move(ground5));
        Grounds.push_back(std::move(ground6));
        Grounds.push_back(std::move(ground7));
        Grounds.push_back(std::move(ground8));

        SetupLevel();
    }

    /*!
     * \brief Cleans up the scene.
     *
     * Deallocates resources and performs any necessary clean-up operations.
     */
    void Cleanup() override
    {
    }

    /*!
     * \brief Checks if the scene is completed.
     * \return True if the scene is completed, false otherwise.
     */
    bool IsCompleted() const override
    {
        return !mRun;
    }

    /*!
     * \brief Checks if the player has won the scene.
     * \return True if the player has won, false otherwise.
     */
    bool IsWin() const override
    {
        return isWin;
    }

    /*!
     * \brief Handles player input for the scene.
     * \param deltaTime The time since the last update.
     *
     * Processes SDL events and updates player states based on input.
     */
    void HandleInput(float deltaTime) override
    {
        SDL_Event event;
        // Processing input
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                SDL_Log("Program quit %u", event.quit.timestamp);
                mRun = false;
            }
        }

        // Handle SDL_GetKeyboardState after -- your SDL_PollEvent
        mainCharacter->Input(deltaTime);
    }

    /*!
     * \brief Renders all entities in the scene.
     *
     * Draws the background, player, enemies, food, and grounds to the screen.
     */
    void Render() override
    {
        SDL_SetRenderDrawColor(mRenderer, 0, 64, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(mRenderer);
        backGround->Render(mRenderer);

        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

        for (int i = 0; i < enemies.size(); i++)
        {
            enemies[i]->Render(mRenderer);
        }
        for (int i = 0; i < foods.size(); i++)
        {
            foods[i]->Render(mRenderer);
        }
        mainCharacter->Render(mRenderer);
        for (int i = 0; i < Grounds.size(); i++)
        {
            Grounds[i]->Render(mRenderer);
        }

        SDL_RenderPresent(mRenderer);
    }

    /*!
     * \brief Updates the state of the scene.
     * \param deltaTime The time since the last update.
     *
     * Updates the positions and states of all entities in the scene. Handles gameplay logic such as collisions
     * and scoring.
     */
    void Update(float deltaTime) override
    {
        auto playerSprite = mainCharacter->GetComponent<SpriteComponent>();
        if (!playerSprite)
        {
            SDL_Log("Player sprite component not found.");
            return;
        }

        float playerBottomY = playerSprite->GetY() + playerSprite->GetHeight();
        bool onGround = false;

        for (auto &ground : Grounds)
        {
            if (mainCharacter->Intersects(ground.get()))
            {
                mainCharacter->SetShouldFall(false);
                onGround = true;
                auto groundSprite = ground->GetComponent<SpriteComponent>();
                if (groundSprite)
                {
                    float groundY = groundSprite->GetY();
                    if (playerBottomY > groundY)
                    {
                        mainCharacter->SetOnGround(groundY - playerSprite->GetHeight());
                    }
                }
                break;
            }
        }
        mainCharacter->Update(deltaTime);

        for (int i = 0; i < foods.size(); i++)
        {
            if (!foods[i])
            {
                continue;
            }
            foods[i]->Update(deltaTime);

            bool playerHitsFood = mainCharacter->Intersects(foods[i].get());

            if (playerHitsFood && foods[i]->IsRenderable())
            {
                foods[i]->SetRenderable(false);
                mPoints += 10.0f;
                SDL_Log("Food eaten. Your score is %f", mPoints);
            }

            bool allFoodsDestroyed = true;
            for (auto &food : foods)
            {
                if (food && food->IsRenderable())
                {
                    allFoodsDestroyed = false;
                    break;
                }
            }

            if (allFoodsDestroyed)
            {
                SDL_Log("YOU WIN!");
                SDL_Log("Your score is %f", mPoints);
                mRun = false;
                isWin = true;
            }
        }
        for (int i = 0; i < enemies.size(); i++)
        {

            bool playerDies = mainCharacter->Intersects(enemies[i].get());

            if (playerDies)
            {
                SDL_Log("YOU LOSE!");
                mRun = false;
            }
        }
        if (!onGround)
        {
            mainCharacter->SetShouldFall(true);
        }
    }

    /*!
     * \brief Sets up the level-specific entities and logic.
     *
     * This method must be implemented by derived classes to create a specific level's layout and gameplay elements.
     */
    virtual void SetupLevel() = 0;
};
