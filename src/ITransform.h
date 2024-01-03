//
// Created by redkc on 02/01/2024.
//

#ifndef OPENGLGP_ITRANSFORM_H
#define OPENGLGP_ITRANSFORM_H


#include "glm/vec2.hpp"

struct Transform {
    glm::vec2 position;
    float rotation;
    glm::vec2 velocity;
    glm::vec2 scale;
};

class ITransform { 
public:
    Transform transform;
    float smooth;//Q: WIll this slow down this A: Yes and what
    float speed;
    
    void moveByMousePos(glm::vec2 mousePosition);
    void moveByInputVector(glm::vec2 mousePosition);
    void translate(float deltaTime = 0.01f);
};


#endif //OPENGLGP_ITRANSFORM_H
