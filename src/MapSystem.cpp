//
// Created by redkc on 03/01/2024.
//

#include <fstream>
#include <iostream>
#include "MapSystem.h"
#include "spdlog/spdlog.h"
#include "Tile.h"

using namespace gl;

void MapSystem::init() {
    // Create vertex array object (VAO)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Set up vertex data
    float vertices[] = {
            // Positions         // Texture coordinates
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VBO and EBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Specify vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Step 3: Open the JSON file
    std::ifstream file("res/levels/1st.json");

    if (!file.is_open()) {
        std::cout << "Failed to open JSON file." << std::endl;
        return;
    }

    // Step 4: Read the contents of the JSON file
    std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Step 5: Close the JSON file
    file.close();

    // Step 6: Parse the JSON data
    if (document.Parse(jsonContent.c_str()).HasParseError()) {
        std::cout << "Failed to parse JSON." << std::endl;
        return;
    }
    spdlog::info("parsedDocument");


    // Step 7: Extract and use the required information from the parsed JSON data
    const rapidjson::Value &loadedTextures = document["textures"];
    if (loadedTextures.IsArray()) {
        for (rapidjson::SizeType i = 0; i < loadedTextures.Size(); i++) {
            const rapidjson::Value &texture = loadedTextures[i];
            if (texture.IsObject()) {
                if (texture.HasMember("path") && texture.HasMember("name")) {
                    std::string path = texture["path"].GetString();
                    std::string name = texture["name"].GetString();
                    std::shared_ptr<Texture> tmpTexture = std::make_shared<Texture>(Texture((path + name).c_str()));
                    tmpTexture->init();
                    textureMap[name] = tmpTexture;
                }
            }
        }
    }

    const rapidjson::Value &loadedTiles = document["tiles"];
    if (loadedTiles.IsArray()) {
        spdlog::info("IsArray!!!");
        for (rapidjson::SizeType i = 0; i < loadedTiles.Size(); i++) {
            const rapidjson::Value &tile = loadedTiles[i];
            if (tile.IsObject()) {
                spdlog::info("IsArray!!!");

                if (tile.HasMember("x") && tile.HasMember("y") && tile.HasMember("texture")) {
                    int x = tile["x"].GetFloat();
                    int y = tile["y"].GetFloat();
                    std::string texture = tile["texture"].GetString();

                    spdlog::info(std::to_string(x) + " " + std::to_string(y));
                    tiles.push_back(*new Tile(shader, textureMap[texture].get(), VAO, Square, true,glm::vec2(x,y),0,glm::vec2(0),glm::vec2(1)));
                }
            }
        }
    }
}

MapSystem::MapSystem(Shader *shader) : shader(shader) {
  
}

void MapSystem::render() {
    for (int i = 0; i < tiles.size(); i++) {
        tiles[i].render();
    }
}
