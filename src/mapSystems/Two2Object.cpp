#include "ICollision.h"
#include "IRenderable.h"
#include "Two2Object.h"

Two2Object::Two2Object(Shader *shader, Texture *texture, GLuint vao, glm::vec2 position, float rotation, glm::vec2 vel, glm::vec2 scale) : IRenderable(
        shader, texture, vao), ITransform(position,rotation,vel,scale) {
    vertexCount = 6;
}
