//
// Created by redkc on 03/01/2024.
//

#include <fstream>
#include <iostream>
#include "MapSystem.h"
#include "spdlog/spdlog.h"
#include "Tile.h"

void MapSystem::init() {
    // Step 3: Open the JSON file
    std::ifstream file("res/levels/1st.json");

    if (!file.is_open()) {
        std::cout << "Failed to open JSON file." << std::endl;
        return ;
    }

    // Step 4: Read the contents of the JSON file
    std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Step 5: Close the JSON file
    file.close();

    // Step 6: Parse the JSON data
    if (document.Parse(jsonContent.c_str()).HasParseError()) {
        std::cout << "Failed to parse JSON." << std::endl;
        return ;
    }
    spdlog::info("parsedDocument");


    // Step 7: Extract and use the required information from the parsed JSON data
    const rapidjson::Value& loadedTextures = document["textures"];
    if (loadedTextures.IsArray()) {
        for (rapidjson::SizeType i = 0; i < loadedTextures.Size(); i++) {
            const rapidjson::Value& texture = loadedTextures[i];
            if (texture.IsObject()) {
                if (texture.HasMember("path") && texture.HasMember("name")) {
                    std::string path = texture["path"].GetString();
                    std::string name = texture["name"].GetString();

                    textureMap[name] = Texture((path + name).c_str());
                }
            }
        }
    }

    const rapidjson::Value& loadedTiles = document["tiles"];
    if (loadedTiles.IsArray()) {
        spdlog::info("IsArray!!!");
        for (rapidjson::SizeType i = 0; i < loadedTiles.Size(); i++) {
            const rapidjson::Value& tile = loadedTiles[i];
            if (tile.IsObject()) {
                spdlog::info("IsArray!!!");

                if (tile.HasMember("x") && tile.HasMember("y") && tile.HasMember("texture")) {
                    int x = tile["x"].GetFloat();
                    int y = tile["y"].GetFloat();
                    std::string texture = tile["texture"].GetString();

                    spdlog::info(std::to_string(x) + " " + std::to_string(y));
                    tiles.push_back(*new Tile(nullptr, t, 0, Square, false));
                }
            }
        }
    }
}

MapSystem::MapSystem() {

}
