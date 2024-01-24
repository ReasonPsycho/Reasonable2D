//
// Created by redkc on 03/01/2024.
//

#ifndef OPENGLGP_BACKGROUNDSYSTEM_H
#define OPENGLGP_BACKGROUNDSYSTEM_H


#include <string>
#include <map>
#include "../cmake-build-debug/_deps/assimp-src/contrib/rapidjson/include/rapidjson/document.h"
#include "Utilities/Texture.h"
#include "Tile.h"
#include "Two2Object.h"
#include "BackgroundLayer.h"

class BackgroundSystem {
public:
    BackgroundSystem(Shader* shader) : shader(shader) { };
    void init();
    void imgui_render();
    Shader *shader;
    std::vector<BackgroundLayer> layers;
    void addLayer(string path,float z,glm::vec3 color,float scroolSpeed);
};


#endif //OPENGLGP_MAPSYSTEM_H
