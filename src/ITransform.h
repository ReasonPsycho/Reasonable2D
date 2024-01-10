//
// Created by redkc on 02/01/2024.
//

#ifndef OPENGLGP_ITRANSFORM_H
#define OPENGLGP_ITRANSFORM_H


#include "glm/vec2.hpp"

struct Transform {
    Transform(glm::vec2 vec1, float d, glm::vec2 vec2, glm::vec2 vec3);

    glm::vec2 position;
    float rotation;
    glm::vec2 velocity;
    glm::vec2 scale;
};

class ITransform { 
public:
    ITransform( glm::vec2 position = glm::vec2(0),
    float rotation  = 0.0f,
    glm::vec2 vel = glm::vec2(0),
    glm::vec2 scale= glm::vec2(0));
    Transform transform;
    float smooth = 0.1f;//Q: WIll this slow down this A: Yes and what
    float speed = 2.5f;
    
    void moveByMousePos(glm::vec2 mousePosition);
    void moveByInputVector(glm::vec2 mousePosition);
    void translate(float deltaTime = 0.01f);
};


#endif //OPENGLGP_ITRANSFORM_H
