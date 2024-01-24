//
// Created by redkc on 03/01/2024.
//

#ifndef OPENGLGP_MAPEDITOR_H
#define OPENGLGP_MAPEDITOR_H


#include <string>
#include <map>
#include "../cmake-build-debug/_deps/assimp-src/contrib/rapidjson/include/rapidjson/document.h"
#include <../cmake-build-debug/_deps/assimp-src/contrib/rapidjson/include/rapidjson/writer.h>
#include <../cmake-build-debug/_deps/assimp-src/contrib/rapidjson/include/rapidjson/prettywriter.h>
#include <../cmake-build-debug/_deps/assimp-src/contrib/rapidjson/include/rapidjson/stringbuffer.h>
#include "Utilities/Texture.h"
#include "Tile.h"



class MapEditor {
public:
    MapEditor(Shader* shader,string path);
    void init();
    Shader *shader;
    std::map<std::string, std::shared_ptr<Texture>> textureMap;
    rapidjson::Document document;
    std::string path;
    
    std::vector<Tile> tiles;
    
    void render();
    void imgui_render(Camera* camera);
    
    std::vector<Tile*> checkForTiles(glm::vec2 pos);

    void createNew();
    void load(string savePath);
    void save(string savePath);
    
    GLuint VAO{}, EBO{}, VBO{};
    
private:
    std::vector<char> buffer;
    std::vector<char> nameBuffer;
    std::vector<char> pathBuffer;
    std::vector<Tile *> openTiles;
    glm::mat4x4 combinedMatrix;
    bool isSelecting = false;
    ImVec2 min;
    ImVec2 max;

    std::vector<Tile *> queryForTilesWithinArea(glm::vec3 vec1, glm::vec3 vec2);
};



#endif //OPENGLGP_MAPSYSTEM_H
