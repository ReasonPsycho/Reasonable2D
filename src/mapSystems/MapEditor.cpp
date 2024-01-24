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
        writer.Key("rotation");
        writer.Double(tile.transform.rotation());  // Assuming the Tile class has rotation() method
        writer.Key("scale");
        writer.StartObject();
        writer.Key("x");
        writer.Double(tile.transform.scale().x);  // Assuming the Tile class has scale() method
        writer.Key("y");
        writer.Double(tile.transform.scale().y);
        writer.EndObject();

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

void MapEditor::createNew() {
    textureMap.clear();
    tiles.clear();
}

void MapEditor::load(string path) {
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
                    float rotation = 0;
                    if (tile.HasMember("rotation")) {
                        rotation = tile["rotation"].GetFloat();
                    }
                    glm::vec2 scale = glm::vec2(1);
                    if (tile.HasMember("scale")) {
                        scale = glm::vec2(tile["scale"]["x"].GetFloat(), tile["scale"]["y"].GetFloat());
                    }
                    std::string texture = tile["texture"].GetString();

                    tiles.push_back(
                            *new Tile(shader, textureMap[texture].get(), VAO, Square, true, glm::vec2(x, y), rotation,
                                      scale, glm::vec2(0)));
                }
            }
        }
    }
}


void MapEditor::imgui_render(Camera *camera) {
    //Clicks

    if (ImGui::IsMouseClicked(1)) {
        openTiles.clear();
        min = ImGui::GetMousePos();
        isSelecting = true;
    }

    if (ImGui::IsMouseDragging(1) && isSelecting) {

    }

    if (ImGui::IsMouseReleased(1) && isSelecting) {
        isSelecting = false;
        max = ImGui::GetMousePos();

        for (auto &tile: tiles) {
            tile.isNodeOpen = false;
        }

        glm::vec3 minPos = camera->ScreenToWorld(glm::vec3(min.x, min.y, -50.0f));
        glm::vec3 maxPos = camera->ScreenToWorld(glm::vec3(max.x, max.y, 50.0f));

        std::vector<Tile *> matchingTiles = queryForTilesWithinArea(minPos, maxPos);

        for (auto *tile: matchingTiles) {
            tile->isNodeOpen = true;
            openTiles.push_back(tile);
        }
    }

    if (ImGui::IsMouseClicked(2) && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
        ImVec2 pos = ImGui::GetMousePos();
        glm::vec2 worldPos = camera->ScreenToWorld(glm::vec3(pos.x, pos.y, -50.0f));
        tiles.emplace_back(shader, textureMap[textureMap.begin()->first].get(), VAO, Square, true,
                           glm::vec2(worldPos.x, worldPos.y), 0,
                           glm::vec2(1), glm::vec2(0));
    }


    //Delere
    if (ImGui::IsKeyPressed(ImGuiKey_Delete)) {
        for (int i = 0; i < tiles.size(); ++i) {
            if (tiles[i].isNodeOpen) {
                tiles.erase(tiles.begin() + i);
                i--;
            }
        }
    }


    if (!openTiles.empty()) {
        static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
        static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

        combinedMatrix = glm::mat4x4(0);
        for (int i = 0; i < openTiles.size(); ++i) {
            combinedMatrix += openTiles[i]->transform.matrix();
        }
        combinedMatrix /= openTiles.size();

        ImGuiIO &io = ImGui::GetIO();
        glm::vec3 snap = glm::vec3(1.0f);
        glm::mat4x4 diff_matrix;

        ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
        ImGuizmo::Manipulate(glm::value_ptr(camera->GetViewMatrix()), glm::value_ptr(camera->GetProjectionMatrix()),
                             mCurrentGizmoOperation, mCurrentGizmoMode, glm::value_ptr(combinedMatrix),
                             glm::value_ptr(diff_matrix), &snap.x);

        for (int i = 0; i < openTiles.size(); ++i) {
            openTiles[i]->transform.setPosition(openTiles[i]->transform.position() + glm::vec2(diff_matrix[3]));
        }
    }


    ImGui::Begin("Map editor");
    std::copy(path.begin(), path.end(), buffer.begin());
    buffer[path.size()] = '\0';
    ImGui::InputText("Level path", buffer.data(), buffer.size());
    path = std::string(buffer.data());

    if (ImGui::Button("Create new")) {
        createNew();
    }

    if (ImGui::Button("Save")) {
        save(path);
    }

    if (ImGui::Button("Load")) {
        load(path);
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
    if(!tiles.empty()){
        if (ImGui::TreeNode("Tiles")) {
            for (auto &tile: tiles) {
                tile.imgui_render(textureMap, camera);
            }
            ImGui::TreePop();
        }; 
    }


    //Ctrl + c
    if (ImGui::IsKeyPressed(ImGuiKey_V)) {
        if (!openTiles.empty()) {
            std::vector<Tile> tilesToInsert;
            for (const auto &tile: openTiles) {
                tilesToInsert.emplace_back(tile->shader, tile->texture, tile->VAO,
                                           tile->collsionType,
                                           tile->isKinematic, tile->transform.position(),
                                           tile->transform.rotation(),tile->transform.scale(), tile->transform.velocity());
            }
            tiles.insert(tiles.end(),tilesToInsert.begin(),tilesToInsert.end());
            openTiles.clear();
            for (int i = 0; i < tiles.size(); ++i) {
                if (tiles[i].isNodeOpen) {
                    openTiles.push_back(&tiles[i]);
                }
            }
        }
    }
    
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

std::vector<Tile *> MapEditor::queryForTilesWithinArea(glm::vec3 vec1, glm::vec3 vec2) {
    std::vector<Tile *> matchingTiles;
    if (vec2.x < vec1.x) {
        float tmp = vec1.x;
        vec1.x = vec2.x;
        vec2.x = tmp;
    }

    if (vec2.y < vec1.y) {
        float tmp = vec1.y;
        vec1.y = vec2.y;
        vec2.y = tmp;
    }
    vec1 -= glm::vec3(0.5f);
    vec2 += glm::vec3(0.5f);

    for (int i = 0; i < tiles.size(); i++) {
        if (tiles[i].transform.position().x > vec1.x && tiles[i].transform.position().y > vec1.y &&
            tiles[i].transform.position().x < vec2.x && tiles[i].transform.position().y < vec2.y) {
            matchingTiles.push_back(&tiles[i]);
        }
    }
    return matchingTiles;
}


