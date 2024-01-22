//
// Created by redkc on 03/01/2024.
//

#ifndef OPENGLGP_TILE_H
#define OPENGLGP_TILE_H


#include "ICollision.h"
#include "IRenderable.h"

class Tile : public ICollision, public IRenderable, public virtual ITransform{
public:
    Tile(Shader *shader, Texture *texture, GLuint vao, CollsionTypeEnum collsionType, bool isKinematic,glm::vec2 position, float rotation, glm::vec2 vel, glm::vec2 scale);
    void imgui_render(std::map<std::string, std::shared_ptr<Texture>> textureMap);
};


#endif //OPENGLGP_TILE_H
