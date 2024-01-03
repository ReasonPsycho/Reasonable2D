//
// Created by redkc on 03/01/2024.
//

#include "Tile.h"

Tile::Tile(Shader *shader, Texture *texture, GLuint vao, CollsionTypeEnum collsionType, bool isKinematic) : IRenderable(
        shader, texture, vao), ICollision(collsionType, isKinematic) {

}
