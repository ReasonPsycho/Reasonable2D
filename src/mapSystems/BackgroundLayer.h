//
// Created by redkc on 22/01/2024.
//

#ifndef OPENGLGP_BACKGROUNDLAYER_H
#define OPENGLGP_BACKGROUNDLAYER_H


#include <vector>
#include <map>
#include "../cmake-build-debug/_deps/assimp-src/contrib/rapidjson/include/rapidjson/document.h"
#include "Tile.h"

class BackgroundLayer {
public:
    BackgroundLayer(Shader* shader,string path, float z, glm::vec3 color,float scrollSpeed) :shader(shader), path(path),z(z),color(color),scrollSpeed(scrollSpeed){
        init();
    }
    std::map<std::string, std::shared_ptr<Texture>> textureMap;
    rapidjson::Document document;
    std::string path;
    Shader *shader;
    GLuint VAO{}, EBO{}, VBO{};
    std::vector<Tile> tiles;
    glm::vec3 color;
    float z;
    float scrollSpeed;
    void render(glm::vec2 pos);
    void imgui_render();
    void init();
    void load(string path);
};


#endif //OPENGLGP_BACKGROUNDLAYER_H
