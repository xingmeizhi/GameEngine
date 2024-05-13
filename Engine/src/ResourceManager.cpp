#include "ResourceManager.h"
#include <SDL2/SDL.h>
#include <iostream>

ResourceManager::ResourceManager() {}

ResourceManager &ResourceManager::GetInstance()
{
    static ResourceManager instance;
    return instance;
}

void ResourceManager::LoadResource(SDL_Renderer *renderer, const std::string &image_filename)
{
    // Check if the resource is already loaded
    if (resources.find(image_filename) != resources.end())
    {
        return;
    }

    // Load the image as a surface
    SDL_Surface *surface = SDL_LoadBMP(image_filename.c_str());
    if (!surface)
    {
        std::cerr << "Failed to load image: " << SDL_GetError() << std::endl;
        return;
    }

    // Convert the surface to a texture
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        return;
    }

    // Store the texture in the resource map
    resources[image_filename] = texture;
}

SDL_Texture *ResourceManager::GetResource(const std::string &key)
{
    auto it = resources.find(key);
    if (it != resources.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

int ResourceManager::StartUp()
{
    SDL_Log("ResourceManager started successfully");
    return 0;
}

int ResourceManager::ShutDown()
{
    // Destroy all textures and clear the resource map
    for (auto &resource : resources)
    {
        SDL_DestroyTexture(resource.second);
    }

    resources.clear();
    SDL_Log("ResourceManager shut down successfully");
    return 0;
}
