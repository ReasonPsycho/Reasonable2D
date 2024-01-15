//
// Created by redkc on 02/01/2024.
//

#include "IRenderable.h"
#include "glm/fwd.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


void IRenderable::render() {
    texture->use(GL_TEXTURE0);
    shader->use();

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(transform.position, -50));
    model = glm::scale(model, glm::vec3(transform.scale,1.0f));
    shader->setMatrix4("transform", false, glm::value_ptr(model));
    shader->setVec3("lightColor",255.0f, 255.0f, 255.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
}

IRenderable::IRenderable(Shader* shader, Texture* texture,GLuint VAO): shader(shader),texture(texture),VAO(VAO) {

}

IRenderable::~IRenderable() {

}

IRenderable::IRenderable(Shader* shader, Texture* texture): shader(shader),texture(texture) {

}
