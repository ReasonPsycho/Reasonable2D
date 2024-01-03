//
// Created by redkc on 02/01/2024.
//

#include "IRenderable.h"
#include "glm/fwd.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


void IRenderable::render() {
    texture->use();
    shader->use();

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(transform.position, -30.0f));
    model = glm::scale(model, glm::vec3(transform.scale,1.0f));
    shader->setMatrix4("transform", false, glm::value_ptr(model));
    shader->setVec3("lightColor", 1.0f, 0.0f, 1.0f);

    glEnable(GL_BLEND); //CHECK make sure that this is right XD 
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 100);
    
    glDisable(GL_BLEND);
}

IRenderable::IRenderable(Shader* shader, Texture* texture,GLuint VAO): shader(shader),texture(texture),VAO(VAO) {

}

IRenderable::~IRenderable() {

}
