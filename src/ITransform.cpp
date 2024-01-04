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

ITransform::ITransform(glm::vec2 position, float rotation, glm::vec2 vel, glm::vec2 scale) : transform(position,rotation,vel,scale){

}

Transform::Transform(glm::vec2 vec1, float d, glm::vec2 vec2, glm::vec2 vec3) {
position = vec1;
rotation = d;
velocity = vec2;
scale = vec3;
}
