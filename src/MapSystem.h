//
// Created by redkc on 03/01/2024.
//

#ifndef OPENGLGP_MAPSYSTEM_H
#define OPENGLGP_MAPSYSTEM_H


#include <string>
#include <map>
#include "../cmake-build-debug/_deps/assimp-src/contrib/rapidjson/include/rapidjson/document.h"
#include "imgui_impl/imgui_impl_opengl3_loader.h"
#include "Utilities/Texture.h"
#include "Tile.h"

class MapSystem {
public:
    MapSystem();
    void init();
    std::map<std::string, Texture> textureMap;
    std::vector<Tile> tiles;
    rapidjson::Document document;
};


#endif //OPENGLGP_MAPSYSTEM_H
