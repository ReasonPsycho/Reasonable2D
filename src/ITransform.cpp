//
// Created by redkc on 02/01/2024.
//

#include "ITransform.h"
#include "glm/common.hpp"

void ITransform::moveByMousePos(glm::vec2 mousePosition) {
    auto diffrenceInX =  mousePosition[0] - transform.position.x;
    auto diffrenceInY =  mousePosition[1] - transform.position.y;
    transform.velocity.x = (glm::clamp(-1.0f,diffrenceInX,1.0f) * (1.0f - smooth) +  transform.velocity.x * smooth) * speed;
    transform.velocity.y = (glm::clamp(-1.0f,diffrenceInY,1.0f)  * (1.0f - smooth) +  transform.velocity.y * smooth)* speed;
    translate();
}

void ITransform::translate(float deltaTime) {
    transform.position.x +=  transform.velocity.x * deltaTime;
    transform.position.y +=  transform.velocity.y * deltaTime;
}

void ITransform::moveByInputVector(glm::vec2 inputVector) {
    transform.velocity.x = (inputVector.x * (1.0f - smooth) + transform.velocity.x * smooth) * speed;
    transform.velocity.y = (inputVector.y * (1.0f - smooth) + transform.velocity.y * smooth) * speed;
    translate();

}
