//
// Created by redkc on 03/01/2024.
//

#ifndef OPENGLGP_TWO2OBJECT_H
#define OPENGLGP_TWO2OBJECT_H


#include "ICollision.h"
#include "IRenderable.h"

class Two2Object : public IRenderable, public virtual ITransform{
public:
    Two2Object(Shader *shader, Texture *texture, GLuint vao,glm::vec2 position, float rotation, glm::vec2 vel, glm::vec2 scale);
};


#endif //OPENGLGP_TILE_H
