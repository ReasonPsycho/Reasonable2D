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
    BackgroundSystem(Shader* shader) : shader(shader){ };
    void init();
    
    Shader *shader;
    std::vector<BackgroundLayer> layers;
   
    void render();
    void addLayer(string path,float z,glm::vec3 color);
};


#endif //OPENGLGP_MAPSYSTEM_H
