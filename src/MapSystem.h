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
    MapSystem(Shader* shader);
    void init();
    Shader *shader;
    std::map<std::string, std::shared_ptr<Texture>> textureMap;
    std::vector<Tile> tiles;
    std::vector<ICollision> collisions;
    rapidjson::Document document;
    void render();
    GLuint VAO{}, EBO{}, VBO{};
};


#endif //OPENGLGP_MAPSYSTEM_H
