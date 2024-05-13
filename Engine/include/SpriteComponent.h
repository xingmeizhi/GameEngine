#pragma once
#include "Component.h"

/*!
 * \struct SpriteComponent
 * \brief The SpriteComponent is responsible for rendering sprites on the screen.
 *
 * Inherits from Component and manages a texture for the sprite, including its position, size, and rendering.
 */
struct SpriteComponent : public Component
{
public:
    /*!
     * \brief Constructs a SpriteComponent with a renderer and a file path to the sprite's texture.
     * \param renderer The SDL renderer to use for rendering the sprite.
     * \param filepath The file path to the texture image.
     *
     * This constructor initializes the sprite component by loading the texture from the specified file path.
     */
    SpriteComponent(SDL_Renderer *renderer, const char *filepath) : mRenderer(renderer)
    {
        SDL_Log("Creating SpriteComponent with file: %s", filepath);
        CreateSprite(filepath);
    }

    /*!
     * \brief Destructor for SpriteComponent.
     */
    virtual ~SpriteComponent()
    {
    }

    /*!
     * \brief Loads the sprite's texture from a file.
     * \param filepath The file path to the texture image.
     *
     * Attempts to load the sprite's texture using the ResourceManager. If the texture is not already loaded,
     * it loads the texture and stores it in the ResourceManager.
     */
    void CreateSprite(const char *filepath)
    {
        ResourceManager &manager = ResourceManager::GetInstance();
        if (manager.GetResource(filepath) == nullptr)
        {
            manager.LoadResource(mRenderer, filepath);
        }
        mTexture = manager.GetResource(filepath);
        if (mTexture == nullptr)
        {
            SDL_Log("Failed to load texture for file: %s", filepath);
        }
    }

    /*!
     * \brief Renders the sprite on the screen.
     * \param renderer The SDL renderer to use for rendering the sprite.
     *
     * If the sprite's texture is loaded, this method renders the sprite at its current position and size.
     */
    void Render(SDL_Renderer *renderer) override
    {
        if (mTexture != nullptr)
        {
            SDL_RenderCopyF(renderer, mTexture, NULL, &mRectangle);
        }
    }

    /*!
     * \brief Sets the sprite's width.
     * \param w The new width of the sprite.
     */
    void SetW(float w)
    {
        mRectangle.w = w;
    }

    /*!
     * \brief Sets the sprite's X position.
     * \param x The new X position of the sprite.
     */
    void SetX(float x)
    {
        mRectangle.x = x;
    }

    /*!
     * \brief Sets the sprite's Y position.
     * \param y The new Y position of the sprite.
     */
    void SetY(float y)
    {
        mRectangle.y = y;
    }

    /*!
     * \brief Gets the sprite's X position.
     * \return The current X position of the sprite.
     */
    float GetX() const
    {
        return mRectangle.x;
    }

    /*!
     * \brief Gets the sprite's Y position.
     * \return The current Y position of the sprite.
     */
    float GetY() const
    {
        return mRectangle.y;
    }

    /*!
     * \brief Moves the sprite to a new position.
     * \param x The new X position of the sprite.
     * \param y The new Y position of the sprite.
     *
     * Updates the sprite's position to the specified X and Y coordinates.
     */
    void Move(float x, float y)
    {
        mRectangle.x = x;
        mRectangle.y = y;
    }

    /*!
     * \brief Gets the rectangle representing the sprite's position and size.
     * \return An SDL_FRect representing the sprite's position and size.
     */
    SDL_FRect GetRectangle() const
    {
        return mRectangle;
    }

    /*!
     * \brief Sets the size of the sprite.
     * \param w The width of the sprite.
     * \param h The height of the sprite.
     *
     * Updates the sprite's width and height to the specified values.
     */
    void SetSize(float w, float h)
    {
        mRectangle.w = w;
        mRectangle.h = h;
    }

    /*!
     * \brief Gets the sprite's width.
     * \return The current width of the sprite.
     */
    float GetWidth() const
    {
        return mRectangle.w;
    }

    /*!
     * \brief Gets the sprite's height.
     * \return The current height of the sprite.
     */
    float GetHeight() const
    {
        return mRectangle.h;
    }

private:
    SDL_FRect mRectangle{20.0f, 20.0f, 32.0f, 32.0f};
    SDL_Texture *mTexture;
    SDL_Renderer *mRenderer;
};
