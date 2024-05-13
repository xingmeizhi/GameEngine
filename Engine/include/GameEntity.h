#pragma once
#include "Component.h"
#include "SpriteComponent.h"

/*!
 * \class GameEntity
 * \brief The GameEntity class serves as a base class for all entities in the game, providing basic functionalities like input handling, updating, and rendering.
 *
 * This class is designed to be extended by specific entities in the game. It holds a collection of components that define the entity's behavior and state. The class supports adding, retrieving, and updating components.
 */
class GameEntity
{
protected:
    std::vector<std::shared_ptr<Component>> components;
    bool mRenderable{true};

public:
    /*!
     * \brief Default constructor for GameEntity.
     */
    GameEntity() = default;

    /*!
     * \brief Virtual destructor for GameEntity.
     */
    virtual ~GameEntity() {}

    /*!
     * \brief Handles input for the entity.
     * \param deltaTime The time since the last update.
     *
     * This method is meant to be overridden by derived classes to handle specific input actions.
     */
    virtual void Input(float deltaTime) {}

    /*!
     * \brief Updates the entity and its components.
     * \param deltaTime The time since the last update.
     *
     * Iterates over all components and updates each one. Can be overridden by derived classes for additional update logic.
     */
    virtual void Update(float deltaTime)
    {
        // SDL_Log("Updating GameEntity");
        for (auto &comp : components)
        {
            comp->Update(deltaTime);
        }
    }

    /*!
     * \brief Renders the entity and its components.
     * \param renderer The SDL renderer to draw to.
     *
     * If the entity is marked as renderable, iterates over all components and renders each one.
     */
    virtual void Render(SDL_Renderer *renderer)
    {
        if (!mRenderable)
        {
            return;
        }

        for (auto &comp : components)
        {
            comp->Render(renderer);
        }
    }

    /*!
     * \brief Sets the entity's renderable state.
     * \param value True if the entity should be rendered, false otherwise.
     */
    void SetRenderable(bool value)
    {
        mRenderable = value;
    }

    /*!
     * \brief Checks if the entity is renderable.
     * \return True if the entity should be rendered, false otherwise.
     */
    bool IsRenderable() const
    {
        return mRenderable;
    }

    /*!
     * \brief Adds a component to the entity.
     * \param args Arguments to pass to the component's constructor.
     * \return A pointer to the newly added component.
     *
     * This method creates a component of type T, adds it to the entity's component list, and returns a pointer to it.
     */
    template <typename T, typename... Args>
    T *AddComponent(Args &&...args)
    {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
        auto comp = std::make_shared<T>(std::forward<Args>(args)...);
        components.push_back(comp);
        return static_cast<T *>(comp.get());
    }

    /*!
     * \brief Retrieves the first component of type T.
     * \return A pointer to the component of type T, or nullptr if not found.
     *
     * This method searches for a component of type T in the entity's component list and returns a pointer to it.
     */
    template <typename T>
    T *GetComponent()
    {
        for (auto &comp : components)
        {
            if (typeid(*comp) == typeid(T))
            {
                return static_cast<T *>(comp.get());
            }
        }
        return nullptr;
    }

    /*!
     * \brief Retrieves the first component of type T, const version.
     * \return A const pointer to the component of type T, or nullptr if not found.
     *
     * This method searches for a component of type T in the entity's component list and returns a const pointer to it.
     */
    template <typename T>
    const T *GetComponent() const
    {
        for (const auto &comp : components)
        {
            if (typeid(*comp) == typeid(T))
            {
                return static_cast<const T *>(comp.get());
            }
        }
        return nullptr;
    }

    /*!
     * \brief Checks if the entity intersects with another entity.
     * \param e The other entity to check intersection with.
     * \return True if there is an intersection, false otherwise.
     *
     * This method checks if the bounding rectangles of the sprite components of two entities intersect.
     */
    bool Intersects(const GameEntity *e) const
    {
        auto ourSprite = GetComponent<SpriteComponent>();
        auto theirSprite = e->GetComponent<SpriteComponent>();

        if (ourSprite && theirSprite)
        {
            SDL_Rect source = ConvertFRectToRect(theirSprite->GetRectangle());
            SDL_Rect us = ConvertFRectToRect(ourSprite->GetRectangle());
            SDL_Rect result;
            if (SDL_IntersectRect(&source, &us, &result))
            {
                return true;
            }
        }

        return false;
    }

    /*!
     * \brief Converts an SDL_FRect to SDL_Rect by casting float values to ints.
     * \param frect The floating point rectangle to convert.
     * \return An SDL_Rect with the converted values.
     *
     * Utility method for converting between SDL rectangle types.
     */
    SDL_Rect ConvertFRectToRect(const SDL_FRect &frect) const
    {
        return SDL_Rect{static_cast<int>(frect.x), static_cast<int>(frect.y), static_cast<int>(frect.w), static_cast<int>(frect.h)};
    }
};