#pragma once
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <unordered_map>
#include <SDL2/SDL.h>

/*!
 * \class ResourceManager
 * \brief Manages the loading, access, and unloading of resources such as textures.
 *
 * The ResourceManager class follows the Singleton design pattern to ensure only one instance manages all resources
 * in the application. It provides methods to load resources from files, retrieve loaded resources, and perform
 * cleanup on shutdown.
 */
class ResourceManager
{
private:
    /*!
     * \brief Private default constructor.
     *
     * Private construction to enforce Singleton pattern. Initializes the resource manager instance.
     */
    ResourceManager();

    /*!
     * \brief Private copy constructor.
     * \param ResourceManager const reference to prevent copying.
     *
     * Deleted to prevent copying of the singleton instance.
     */
    ResourceManager(ResourceManager const &);

    /*!
     * \brief Overloaded assignment operator.
     * \param ResourceManager const reference to prevent assignment.
     *
     * Deleted to prevent assignment of the singleton instance.
     */
    void operator=(ResourceManager const &);

    /*!
     * \brief Container for storing loaded resources.
     *
     * Maps resource identifiers to SDL_Texture pointers. Ensures each resource is only loaded once.
     */
    std::unordered_map<std::string, SDL_Texture *> resources;

public:
    /*!
     * \brief Retrieves the singleton instance of ResourceManager.
     * \return Reference to the singleton ResourceManager instance.
     *
     * Provides global access to the singleton instance of the resource manager.
     */
    static ResourceManager &GetInstance();

    /*!
     * \brief Loads a texture resource.
     * \param renderer Pointer to the SDL_Renderer to use for texture creation.
     * \param image_filename The path to the image file to load.
     *
     * Loads an image from the specified file path and creates an SDL_Texture from it. The texture is stored
     * in the resources container.
     */
    void LoadResource(SDL_Renderer *renderer, const std::string &image_filename);

    /*!
     * \brief Retrieves a loaded texture resource.
     * \param key The identifier of the resource to retrieve.
     * \return Pointer to the SDL_Texture associated with the key, or nullptr if not found.
     *
     * Looks up a resource by its identifier and returns a pointer to the SDL_Texture if it exists.
     */
    SDL_Texture *GetResource(const std::string &key);

    /*!
     * \brief Initializes the ResourceManager.
     *
     * Initializes the ResourceManager.
     */
    int StartUp();

    /*!
     * \brief Performs cleanup tasks for the ResourceManager.
     *
     * Frees all loaded resources and prepares the resource manager for shutdown.
     */
    int ShutDown();
};

#endif // RESOURCE_MANAGER_H
