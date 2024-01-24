//
// Created by redkc on 03/01/2024.
//

#ifndef OPENGLGP_MAPSYSTEM_H
#define OPENGLGP_MAPSYSTEM_H


#include <string>
#include <map>
#include "../cmake-build-debug/_deps/assimp-src/contrib/rapidjson/include/rapidjson/document.h"
#include "Utilities/Texture.h"
#include "Tile.h"

class MapSystem {
public:
    MapSystem(Shader* shader,string path);
    void init();
    glm::vec2 randomPositionAtEgedeOfTheMap();
    glm::vec2 closestGoal(glm::vec2 currentPos);
    Shader *shader;
    std::map<std::string, std::shared_ptr<Texture>> textureMap;
    rapidjson::Document document;
    std::string path;
    
    std::vector<Tile> tiles;
    std::vector<ICollision> collisions;
    std::vector<ICollision> goals;
    void render();
    void load(string path);
    GLuint VAO{}, EBO{}, VBO{};
};


#endif //OPENGLGP_MAPSYSTEM_H
