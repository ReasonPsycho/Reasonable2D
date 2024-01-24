//
// Created by redkc on 03/01/2024.
//

#ifndef OPENGLGP_TILE_H
#define OPENGLGP_TILE_H


#include "ICollision.h"
#include "IRenderable.h"

class Tile : public ICollision, public IRenderable, public virtual ITransform{
public:
    bool isNodeOpen = false;
    Tile(Shader *shader, Texture *texture, GLuint vao, CollsionTypeEnum collsionType, bool isKinematic,glm::vec2 position, float rotation,glm::vec2 scale, glm::vec2 vel );
    void imgui_render(std::map<std::string, std::shared_ptr<Texture>> textureMap,Camera* camera);
};


#endif //OPENGLGP_TILE_H
