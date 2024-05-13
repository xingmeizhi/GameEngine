#pragma once
#include <SDL2/SDL.h>

/*!
 * \class Component
 * \brief The Component class is an abstract base class for all components.
 *
 * This class defines the interface for components, provides a common interface for updating and rendering, which derived
 * components can override to implement their specific behavior.
 */
class Component
{
public:
    virtual ~Component() = default;
    virtual void Update(float deltaTime) {}
    virtual void Render(SDL_Renderer *renderer) {}
};
