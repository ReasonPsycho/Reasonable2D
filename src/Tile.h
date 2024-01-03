//
// Created by redkc on 03/01/2024.
//

#ifndef OPENGLGP_TILE_H
#define OPENGLGP_TILE_H


#include "ICollision.h"
#include "IRenderable.h"

class Tile : public ICollision, IRenderable{
public:
    Tile(Shader *shader, Texture *texture, GLuint vao, CollsionTypeEnum collsionType, bool isKinematic);
};


#endif //OPENGLGP_TILE_H
