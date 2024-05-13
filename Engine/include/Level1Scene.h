#pragma once
#include "BaseScene.h"
#include "ConfigManager.h"

/*!
 * \class Level1Scene
 * \brief The Level1Scene class sets up and manages the first level of the game.
 *
 * Inherits from BaseScene and implements the SetupLevel method to create a specific game level, including
 * initializing enemies and food items at positions defined in a configuration file.
 */
class Level1Scene : public BaseScene
{
    using BaseScene::BaseScene;

    /*!
     * \brief Sets up the level-specific entities and environment.
     *
     * Overrides the SetupLevel method from BaseScene to initialize the level with enemies and food items
     * based on positions defined in the "Config/level1_config.txt" file. Utilizes the ConfigManager to
     * load the configuration and create game entities accordingly.
     */
    void SetupLevel() override
    {
        ConfigManager configManager;
        auto config = configManager.LoadConfig("Config/level1_config.txt");

        // Create enemies based on config
        int enemyIndex = 1;
        while (config.find("enemy" + std::to_string(enemyIndex) + "_x") != config.end())
        {
            auto enemy = std::make_unique<EnemyGameEntity>(mRenderer);
            float x = config["enemy" + std::to_string(enemyIndex) + "_x"];
            float y = config["enemy" + std::to_string(enemyIndex) + "_y"];
            enemy->GetComponent<SpriteComponent>()->Move(static_cast<int>(x), static_cast<int>(y));
            enemies.push_back(std::move(enemy));
            enemyIndex++;
        }

        // Create foods based on config
        int foodIndex = 1;
        while (config.find("food" + std::to_string(foodIndex) + "_x") != config.end())
        {
            auto food = std::make_unique<FoodGameEntity>(mRenderer);
            float x = config["food" + std::to_string(foodIndex) + "_x"];
            float y = config["food" + std::to_string(foodIndex) + "_y"];
            food->GetComponent<SpriteComponent>()->Move(static_cast<int>(x), static_cast<int>(y));
            foods.push_back(std::move(food));
            foodIndex++;
        }
    }
};
