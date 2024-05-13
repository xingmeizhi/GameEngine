#pragma once
#include "Component.h"
#include "ResourceManager.h"
#include "EnemyGameEntity.h"
#include "PlayerGameEntity.h"
#include "SpriteComponent.h"
#include "SceneManager.h"
#include "Level1Scene.h"
#include <SDL2/SDL.h>
#include <cstdlib>
#include <vector>
#include <memory>

/*!
 *  \class Application
 *  \brief The Application class is responsible for initializing, running, and shutting down the game application.
 *
 *  This class encapsulates the game's main loop, including initialization of SDL, the game window, and renderer,
 *  as well as managing the game's state through the SceneManager.
 */
class Application
{
public:
    /*!
     *  \brief Constructor that initializes the game application.
     *  \param w The width of the game window.
     *  \param h The height of the game window.
     *
     *  This constructor initializes SDL, creates the game window and renderer, and switches to the initial game scene.
     */
    Application(int w, int h)
    {
        StartUp(w, h);
        sceneManager.SwitchScene(std::make_unique<Level1Scene>(mRenderer, mWindow));
    }
    /*!
     *  \brief Destructor that shuts down the game application.
     *
     *  This destructor handles the clean-up and resource deallocation for the game application.
     */
    ~Application()
    {
        Shutdown();
    }

    /*!
     *  \brief Initializes the SDL library, creates the game window and renderer.
     *  \param w The width of the game window.
     *  \param h The height of the game window.
     *
     *  This method initializes SDL and sets up the game window and renderer. If SDL cannot be initialized or the renderer cannot be created, it logs an error.
     */
    void StartUp(int w, int h)
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
        {
            SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        }
        // Create our window
        mWindow = SDL_CreateWindow("Food chain", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, 0);
        mRenderer = SDL_CreateRenderer(mWindow, NULL, SDL_RENDERER_ACCELERATED);
        if (nullptr == mRenderer)
        {
            SDL_Log("Error creating renderer");
        }
    }

    /*!
     *  \brief Shuts down the game application, releasing all resources.
     *
     *  This method deallocates all allocated resources, destroys the game window, and quits SDL.
     */
    void Shutdown()
    {
        ResourceManager &manager = ResourceManager::GetInstance();
        manager.ShutDown();
        SDL_DestroyWindow(mWindow);
        SDL_Quit();
    }

    /*!
     *  \brief Runs the main loop of the game application.
     *  \param targetFPS The target frames per second (FPS) the game tries to maintain.
     *
     *  This method runs the game's main loop, which includes updating the game state, handling input, rendering, and regulating the game's frame rate.
     */
    void Loop(float targetFPS)
    {
        // Our infinite game/application loop

        Uint64 lastTime, currentTime;

        // Record our 'starting time'
        lastTime = SDL_GetTicks();
        Uint64 framesElapsed = 0;
        float deltaTime = 1.0f / targetFPS;
        while (sceneManager.GetCurrentScene()->IsCompleted() == false)
        {
            Uint64 startOfFrame = SDL_GetTicks();
            // We want, input/update/render to take 16ms
            sceneManager.Update(deltaTime);
            sceneManager.HandleInput(deltaTime);
            sceneManager.Render();
            Uint64 elapsedTime = SDL_GetTicks() - startOfFrame;
            framesElapsed++;

            // Time keeping code - for frames elapsed
            currentTime = SDL_GetTicks();
            // Insert a 'frame cap' so that our program
            // does not run too fast.
            if (elapsedTime < (1000 / targetFPS))
            {
                Uint64 delay = (1000 / targetFPS) - elapsedTime;
                SDL_Delay(delay);
                //                SDL_Log("elaspedTime: %li",elapsedTime);
                //                SDL_Log("delaying by: %li",delay);
            }
            // If 1 second passes,              enemies[i]->report how many frames
            // have been executed.
            if (currentTime > lastTime + 1000)
            {
                deltaTime = 1.0f / framesElapsed;
                framesElapsed = 0;
                lastTime = SDL_GetTicks();
            }
            if (sceneManager.GetCurrentScene()->IsWin())
            {
                sceneManager.LoadNextLevel(mRenderer, mWindow);
                if (sceneManager.GetCurrentScene()->IsCompleted())
                {
                    break;
                }
            }
        }
    }

private:
    // Enemy sprites
    std::vector<std::unique_ptr<EnemyGameEntity>> enemies;
    // Main Character
    std::unique_ptr<PlayerGameEntity> mainCharacter;
    SceneManager sceneManager;
    bool mRun{true};
    float mPoints{0.0f};
    SDL_Window *mWindow;
    SDL_Renderer *mRenderer;
};
