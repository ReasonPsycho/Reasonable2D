//
// Created by redkc on 03/01/2024.
//

#include <fstream>
#include <iostream>
#include "MapEditor.h"
#include "spdlog/spdlog.h"
#include "Tile.h"
#include "glm/gtc/random.hpp"
#include "imgui.h"


void MapEditor::init() {
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

    load();

}

MapEditor::MapEditor(Shader *shader, string path) : shader(shader), path(path), buffer(100), nameBuffer(100),
                                                    pathBuffer(100) {

}

void MapEditor::render() {
    for (int i = 0; i < tiles.size(); i++) {
        tiles[i].render();
    }
}

void MapEditor::save(string savePath) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

// Start writing the document
    writer.StartObject();

// Write textures
    writer.Key("textures");
    writer.StartArray();
    for (const auto &texturePair: textureMap) {
        writer.StartObject();
        writer.Key("path");
        writer.String(texturePair.second->directory.c_str());  // Assuming the Texture class has getPath() method
        writer.Key("name");
        writer.String(texturePair.second->name.c_str());  // Assuming the Texture class has getName() method
        writer.EndObject();
    }
    writer.EndArray();

// Write tiles
    writer.Key("tiles");
    writer.StartArray();
    for (const auto &tile: tiles) {
        writer.StartObject();
        writer.Key("x");
        writer.Double(tile.transform.position().x);  // Assuming the Tile class has getX() method
        writer.Key("y");
        writer.Double(tile.transform.position().y);  // Assuming the Tile class has getY() method
        writer.Key("texture");
        writer.String(tile.texture->name.c_str());  // Assuming the Tile class has getTextureName() method
        writer.EndObject();
    }
    writer.EndArray();

// Finish writing the document
    writer.EndObject();

// Open output stream and write to file
    std::ofstream outFile(savePath);
    if (!outFile.is_open()) {
        std::cout << "Failed to open JSON file." << std::endl;
        return;
    }
    outFile << buffer.GetString();
    outFile.close();
}

void MapEditor::load() {
    textureMap.clear();
    tiles.clear();
    // Step 3: Open the JSON file
    std::ifstream file(path);

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
                    std::shared_ptr<Texture> tmpTexture = std::make_shared<Texture>(name, path, "IDK");
                    textureMap[name] = tmpTexture;
                }
            }
        }
    }

    const rapidjson::Value &loadedTiles = document["tiles"];
    if (loadedTiles.IsArray()) {
        for (rapidjson::SizeType i = 0; i < loadedTiles.Size(); i++) {
            const rapidjson::Value &tile = loadedTiles[i];
            if (tile.IsObject()) {

                if (tile.HasMember("x") && tile.HasMember("y") && tile.HasMember("texture")) {
                    int x = tile["x"].GetFloat();
                    int y = tile["y"].GetFloat();
                    std::string texture = tile["texture"].GetString();

                    tiles.push_back(*new Tile(shader, textureMap[texture].get(), VAO, Square, true, glm::vec2(x, y), 0,
                                              glm::vec2(0), glm::vec2(1)));
                }
            }
        }
    }
}


void MapEditor::imgui_render(Camera *camera) {
    if (ImGui::IsMouseClicked(1) && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
        ImVec2 pos = ImGui::GetMousePos();
        std::vector<Tile *> matchingTiles = checkForTiles(camera->ScreenToWorld(glm::vec3(pos.x, pos.y, -50.0f)));

        if (!matchingTiles.empty()) {
            for (int i = 0; i < tiles.size(); ++i) {
                tiles[i].isNodeOpen = false;
            }
        }
        for (int i = 0; i < matchingTiles.size(); ++i) {
            matchingTiles[i]->isNodeOpen = true;
        }
    }
    ImGui::Begin("Map editor");
    std::copy(path.begin(), path.end(), buffer.begin());
    buffer[path.size()] = '\0';
    ImGui::InputText("Level path", buffer.data(), buffer.size());
    path = std::string(buffer.data());

    if (ImGui::Button("Save")) {
        save(path);
    }

    if (ImGui::Button("Load")) {
        load();
    }

    if (ImGui::TreeNode("Textures")) {
        for (auto &nameTexture: textureMap) {
            ImGui::Text(nameTexture.first.c_str());
        }
        // Create a buffer with a maximum size of your choice.

        ImGui::InputText("Texture name", nameBuffer.data(), nameBuffer.size());
        ImGui::InputText("Texture path", pathBuffer.data(), pathBuffer.size());
        if (ImGui::Button("Add texture")) {
            textureMap[nameBuffer.data()] = make_shared<Texture>(nameBuffer.data(), pathBuffer.data(), "");
        }

        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Tiles")) {
        for (auto &tile: tiles) {
            tile.imgui_render(textureMap, camera);
        }
        ImGui::TreePop();
    };

    ImGui::End();
}

std::vector<Tile *> MapEditor::checkForTiles(glm::vec2 pos) {
    std::vector<Tile *> matchingTiles;
    for (int i = 0; i < tiles.size(); i++) {

        if (glm::distance(tiles[i].transform.position(), pos) < 0.5f) {
            matchingTiles.push_back(&tiles[i]);
        }
    }
    return matchingTiles;
}


