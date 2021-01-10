#pragma once

#pragma warning (push, 0)

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <vector>

#include <boost/json.hpp>

#pragma warning (pop)

/**
 * \author Magdalena Majkowska
 * 
 * JSONParser helps serialize and deserialize JSON objects in our project
 */
class JsonParser
{
public:
    inline static const std::string SAVE_PATH = R"(.\config\creature\)";
    static std::vector<std::filesystem::path> searchFiles(const std::string &pathToFolder)
    {
        std::vector<std::filesystem::path> filenames;
        std::filesystem::path path = pathToFolder;
        if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
        {
            for (const auto &entry : std::filesystem::directory_iterator(path))
            {
                filenames.push_back(entry.path());
            }
        }
        return filenames;
    }

    static boost::json::object getValueFromFile(std::filesystem::path path)
    {
        std::ifstream stream;
        stream.open(path.string(), std::ios::in);
        std::stringstream buffer;
        buffer << stream.rdbuf();
        boost::json::value jv = boost::json::parse(buffer.str());
        return jv.as_object();
    }

    static std::string saveJsonToFile(std::string filename, std::string jsonToSave)
    {
        int index = 0;
        std::string testFilename = filename;
        std::string fullPath = SAVE_PATH + testFilename + ".json";
        std::filesystem::path path = fullPath;
        while (std::filesystem::exists(path))
        {
            std::string testFilename = filename;
            testFilename += std::to_string(index);
            index++;
            fullPath = SAVE_PATH + testFilename + ".json";
            path = fullPath;
        }
        std::ofstream fileStream(path);
        fileStream << jsonToSave;
        fileStream.close();
        return fullPath;
    }
};
