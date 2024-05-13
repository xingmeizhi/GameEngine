#pragma once
#include <memory>
#include "Scene.h"
#include <SDL2/SDL.h>
#include "Application.hpp"
#include "Level1Scene.h"
#include "Level2Scene.h"
#include "Level3Scene.h"
#include "ResourceManager.h"

/*!
 * \class SceneManager
 * \brief The SceneManager class manages the current scene of the game and transitions between scenes.
 *
 * It holds a pointer to the currently active scene and provides methods to switch to a new scene, forward input to the
 * current scene, and update and render the current scene. The SceneManager ensures that the lifecycle methods of the
 * scenes are called appropriately.
 */
class SceneManager
{
private:
    std::unique_ptr<Scene> currentScene;
    int currentLevelIndex = 0;

public:
    SceneManager() = default;
    ~SceneManager() = default;

    /*!
     * \brief Switches to a new scene, performing cleanup on the old scene and initialization on the new one.
     * \param newScene The new scene to switch to.
     */
    void SwitchScene(std::unique_ptr<Scene> newScene)
    {
        if (currentScene)
        {
            currentScene->Cleanup();
        }
        currentScene = std::move(newScene);
        currentScene->Init();
    }

    /*!
     * \brief Forwards input to the current scene.
     * \param deltaTime The time since the last frame in seconds.
     */
    void HandleInput(float deltaTime)
    {
        if (currentScene)
        {
            currentScene->HandleInput(deltaTime);
        }
    }

    /*!
     * \brief Updates the current scene.
     * \param deltaTime The time since the last frame in seconds.
     */
    void Update(float deltaTime)
    {
        if (currentScene)
        {
            currentScene->Update(deltaTime);
        }
    }

    /*!
     * \brief Renders the current scene.
     */
    void Render()
    {
        if (currentScene)
        {
            currentScene->Render();
        }
    }
    /*!
     * \brief Returns a pointer to the current scene.
     * \return A pointer to the current scene.
     */
    Scene *GetCurrentScene() const
    {
        return currentScene.get();
    }

    /*!
     * \brief Loads the next level based on the current level index.
     * \param renderer The SDL renderer used for scene rendering.
     * \param window The SDL window where the scene is rendered.
     *
     * Advances the level index and switches to the corresponding scene.
     */
    void LoadNextLevel(SDL_Renderer *renderer, SDL_Window *window)
    {
        currentLevelIndex++;
        switch (currentLevelIndex)
        {
        case 1:
            SwitchScene(std::make_unique<Level2Scene>(renderer, window));
            break;
        case 2:
            SwitchScene(std::make_unique<Level3Scene>(renderer, window));
            break;
        }
    }
};
