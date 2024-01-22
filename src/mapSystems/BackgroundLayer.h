//
// Created by redkc on 22/01/2024.
//

#ifndef OPENGLGP_BACKGROUNDLAYER_H
#define OPENGLGP_BACKGROUNDLAYER_H


#include <vector>
#include <map>
#include "Two2Object.h"
#include "../cmake-build-debug/_deps/assimp-src/contrib/rapidjson/include/rapidjson/document.h"

class BackgroundLayer {
public:
    std::map<std::string, std::shared_ptr<Texture>> textureMap;
    rapidjson::Document document;
    std::string path;
    Shader *shader;
    GLuint VAO{}, EBO{}, VBO{};
    std::vector<Two2Object> objects;
    float z;
    float scroll;
    void render();
    void init();
};


#endif //OPENGLGP_BACKGROUNDLAYER_H
