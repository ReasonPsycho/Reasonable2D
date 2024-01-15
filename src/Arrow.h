//
// Created by redkc on 03/01/2024.
//

#ifndef OPENGLGP_ARROW_H
#define OPENGLGP_ARROW_H


#include "ICollision.h"
#include "IRenderable.h"

class Arrow : public ICollision, public IRenderable, public virtual ITransform{
public:
    Arrow(Shader *shader, Texture *texture, GLuint vao, CollsionTypeEnum collsionType, bool isKinematic,glm::vec2 position, float rotation, glm::vec2 vel, glm::vec2 scale);
};


#endif //OPENGLGP_TILE_H
