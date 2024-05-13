#pragma once

#include <string>
#include <fstream>
#include <unordered_map>

/*!
 * \class ConfigManager
 * \brief The ConfigManager class provides functionalities to load configuration settings from a file.
 *
 * This class is designed to read configuration files where each line defines a key-value pair, separated by an
 * equals sign (=). Lines starting with '#' or ';' are considered comments and ignored. The class supports
 * integer values for the configuration settings.
 */
class ConfigManager
{
public:
    /*!
     * \brief Loads configuration settings from a specified file.
     * \param filePath The path to the configuration file.
     * \return An unordered_map containing the configuration settings.
     *
     * Reads the configuration file line by line, ignoring empty lines and comments. Each non-comment line is
     * expected to contain a key-value pair separated by an equals sign (=). The method parses these lines,
     * converting the value part to an integer, and stores them in an unordered_map.
     */
    std::unordered_map<std::string, int> LoadConfig(const std::string &filePath)
    {
        std::unordered_map<std::string, int> config;
        std::ifstream file(filePath);
        std::string line;

        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '#' || line[0] == ';')
                continue;
            auto delimiterPos = line.find('=');
            if (delimiterPos != std::string::npos)
            {
                std::string key = line.substr(0, delimiterPos);
                int value = std::stoi(line.substr(delimiterPos + 1));
                config[key] = value;
            }
        }

        return config;
    }
};
