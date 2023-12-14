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
    Crircle(glm::vec2 pos,glm::vec2 vec,float radius,float speed);
    ~Crircle();

    void init();
    void init(GLuint newVBO,GLuint newVAO,GLuint newEBO);
    void render(Shader* shaderProgram, Texture* texture);
    void move(float deltaTime);
    int detectCollisions(std::vector<Crircle> &circles, int thisCricleIndex, bool doesSeperate, bool doesBounce);
    void changeMovment(bool doesSeperate, bool doesBounce, bool doesWallsSeperate, bool doesWallsBounce);
    bool checkCollisionCircleRectangle(glm::vec4 reactPoints, bool doesSeperate, bool doesBounce);
    bool collide(std::vector<Crircle> &circles, int thisCricleIndex, bool doesBallsCollideWithBalls,
                 bool doesSeperateFromBalls);
    bool operator==(const Crircle& c) const;
    
    glm::vec2 position;
    glm::vec2 velocity;
    float radius;
    float speed;
    GLuint VBO, VAO, EBO;
    std::vector<Crircle> collided;
private:
    static int nextID; // Static variable to keep track of the next available ID

    glm::vec2  meanSeparationVector;
    glm::vec2  wallsSeparationVector;
    int uniqueID;      // Instance variable to store the unique ID for each object
};


#endif //OPENGLGP_CRIRCLE_H
