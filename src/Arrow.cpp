//
// Created by redkc on 03/01/2024.
//

#include "Arrow.h"

Arrow::Arrow(Shader *shader, Texture *texture, GLuint vao, CollsionTypeEnum collsionType, bool isKinematic,glm::vec2 position, float rotation, glm::vec2 vel, glm::vec2 scale) : IRenderable(
        shader, texture, vao), ICollision(collsionType, isKinematic,scale), ITransform(position,rotation,vel,scale) {
    vertexCount = 6;
}

