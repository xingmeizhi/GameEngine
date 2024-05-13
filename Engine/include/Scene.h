#pragma once

/*!
 * \class Scene
 * \brief The Scene class is an abstract base class for defining various scenes in the game.
 *
 * This interface mandates the implementation of core methods essential for scene lifecycle management, including
 * initialization, input handling, updating, rendering, and cleanup. Additionally, it provides methods to check the
 * completion status of the scene and whether the player has won.
 */
class Scene
{
public:
    virtual ~Scene() = default;
    virtual void Init() = 0;
    virtual void StartUp() = 0;
    virtual void HandleInput(float deltaTime) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
    virtual void Cleanup() = 0;
    virtual bool IsCompleted() const = 0;
    virtual bool IsWin() const = 0;
};
