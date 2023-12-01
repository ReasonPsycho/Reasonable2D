//
// Created by redkc on 29/11/2023.
//

#ifndef OPENGLGP_CRIRCLE_H
#define OPENGLGP_CRIRCLE_H

#include <vector>
#include "Utilities/Shader.h"
#include "Utilities/Texture.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Crircle {
public:
    Crircle(glm::vec2 pos,glm::vec2 vel);
    ~Crircle();

    void init();
    void init(GLuint newVBO,GLuint newVAO,GLuint newEBO);
    void render(Shader* shaderProgram, Texture* texture);
    void move();
    void detectCollisions(const std::vector<Crircle>& circles);
    bool checkCollisionCircleRectangle(glm::vec4 reactPoints);
    void collide(bool doesBallsCollideWithEachOther,const std::vector<Crircle> &circles);

    glm::vec2 position;
    glm::vec2 velocity;
    GLuint VBO, VAO, EBO;
private:
    std::vector<Crircle> collided;
};


#endif //OPENGLGP_CRIRCLE_H
