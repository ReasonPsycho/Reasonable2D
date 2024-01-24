//
// Created by redkc on 02/01/2024.
//

#ifndef OPENGLGP_ITRANSFORM_H
#define OPENGLGP_ITRANSFORM_H


#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtx/quaternion.hpp"
#include "imgui.h"
#include "../../../../cmake-build-debug/_deps/imguizmo-src/ImGuizmo.h"
#include "Camera.h"

struct Transform {
    
    Transform(glm::vec2 position = glm::vec2(0.0f), float rotation = 0,glm::vec2 vel = glm::vec2(0.0f),
              glm::vec2 scale = glm::vec2(1.0f))
            : positionVec(position), rotationVal(rotation), scaleVec(scale) ,velocityVec(vel) {
        updateMatrix();
    }

    glm::vec2 position() const {
        return positionVec;
    }

    void setPosition(const glm::vec2 &position) {
        positionVec = position;
        updateMatrix();
    }

    float rotation() const {
        return rotationVal;
    }

    void setRotation(float rotation) {
        rotationVal = rotation;
        updateMatrix();
    }

    glm::vec2 scale() const {
        return scaleVec;
    }

    void setScale(const glm::vec2 &scale) {
        scaleVec = scale;
        updateMatrix();
    }

    glm::vec2 velocity() const {
        return velocityVec;
    }

    void setVelocity(const glm::vec2 &velocity) {
        velocityVec = velocity;
    }


    glm::mat4x4 matrix() const {
        return matrixVal;
    }

    void setMatrix(const glm::mat4x4 &matrix) {
        matrixVal = matrix;
        updateValuesFromMatrix();
    }

private:
    glm::vec2 positionVec;
    float rotationVal;
    glm::vec2 scaleVec;
    glm::vec2 velocityVec;
    glm::mat4x4 matrixVal;
    
    void updateMatrix() {
        matrixVal = glm::mat4x4(1.0f);
        matrixVal = glm::translate(matrixVal, glm::vec3(positionVec, -50.0f));
        matrixVal = glm::rotate(matrixVal, rotationVal, glm::vec3(0.0f, 0.0f, 1.0f));
        matrixVal = glm::scale(matrixVal, glm::vec3(scaleVec, 1.0f));
    }

    void updateValuesFromMatrix() {
        positionVec = glm::vec2(matrixVal[3]);
        glm::quat q = glm::quat_cast(matrixVal);
        glm::vec3 eulerAngles = glm::eulerAngles(q);
        rotationVal = eulerAngles.z + ((abs(eulerAngles.x) == M_PI / 2 && abs(eulerAngles.y) == M_PI / 2) ? (M_PI / 2) : 0);
        scaleVec.x = glm::length(glm::vec2(matrixVal[0]));
        scaleVec.y = glm::length(glm::vec2(matrixVal[1]));
    }
};

class ITransform { 
public:
    ITransform( glm::vec2 position = glm::vec2(0),
    float rotation  = 0.0f,
    glm::vec2 vel = glm::vec2(0),
    glm::vec2 scale= glm::vec2(0));
    Transform transform;
    float smooth = 0.1f;//Q: WIll this slow down this A: Yes and what
    float speed = 5.0f;
    void moveByMousePos(glm::vec2 mousePosition);
    void moveByInputVector(glm::vec2 mousePosition,float deltaTime);
    void translate(float deltaTime = 0.01f);
    void imgui_render(Camera* camera);
};


#endif //OPENGLGP_ITRANSFORM_H
