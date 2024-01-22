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
    BackgroundSystem(Shader* shader,string path);
    void init();
    
    Shader *shader;
    std::map<std::string, std::shared_ptr<Texture>> textureMap;
   
    std::vector<BackgroundLayer> layers;
   
    void render();
};


#endif //OPENGLGP_MAPSYSTEM_H
