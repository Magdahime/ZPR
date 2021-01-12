#pragma once

#ifndef LINUX_PRAGMA
#pragma warning(push, 0)
#endif //LINUX_PRAGMA

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>

#include <boost/json.hpp>

#ifndef LINUX_PRAGMA
#pragma warning(pop)
#endif //LINUX_PRAGMA

/**
 * \author Magdalena Majkowska
 * 
 * JSONParser helps serialize and deserialize JSON objects in our project. All of the 
 * methods are static because they do not require access to any fields of the class.
 * 
 */
class JsonParser
{
private:
    JsonParser(const JsonParser &) = delete;
    JsonParser(JsonParser &&) = delete;
    JsonParser &operator=(JsonParser &&) = delete;
    JsonParser &operator=(const JsonParser &) = delete;

public:
    inline static const std::string SAVE_PATH = R"(.\config\creature\)";

    /**
     * Method to retrieve all of the files from specified folder.
     * \param pathToFolder Path to folder that we want to search
     * \return Vector of std::filesystem::path objects
     */
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
    /**
     * Method to read JSON file and creates a boost::json::object for further processing. 
     * \param path Path to JSON file (std::filesystem::path not string)
     * \return boost::json::object that we can easily read to get data
     */
    static boost::json::object getValueFromFile(std::filesystem::path path)
    {
        std::ifstream stream;
        stream.open(path.string(), std::ios::in);
        std::stringstream buffer;
        buffer << stream.rdbuf();
        std::string toParse = buffer.str();
        std::replace(toParse.begin(), toParse.end(), '\'', ' ');
        boost::json::value jv = boost::json::parse(toParse);
        return jv.as_object();
    }
    /**
     * Method to save a prepared beforehand string to file. 
     * Used for saving a new user-defined specimens in webview.  
     * \param filename Name for the file, that will store new Creature Parameters
     * \param jsonToSave JSON string
     * \return Full path to this filename (only for purposes of testing)
     */
    static std::string saveJsonToFile(std::string filename, std::string jsonToSave)
    {
        size_t index = 0;
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
