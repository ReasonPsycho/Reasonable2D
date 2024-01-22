//
// Created by redkc on 02/01/2024.
//

#include <sstream>
#include <iomanip>
#include <valarray>
#include "ITransform.h"
#include "glm/common.hpp"
#include "spdlog/spdlog.h"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

void ITransform::moveByMousePos(glm::vec2 mousePosition) {
    auto diffrenceInX =  mousePosition[0] / 2 - transform.position.x ;
    auto diffrenceInY =  mousePosition[1] / 2 - transform.position.y ;
    
    transform.velocity.x = (glm::clamp(-1.0f,diffrenceInX,1.0f) * (1.0f - smooth) +  transform.velocity.x * smooth) * speed;
    transform.velocity.y = (glm::clamp(-1.0f,diffrenceInY,1.0f)  * (1.0f - smooth) +  transform.velocity.y * smooth)* speed;
    translate();

    
}

void ITransform::translate(float deltaTime ) {
    transform.position.x +=  transform.velocity.x * deltaTime;
    transform.position.y +=  transform.velocity.y * deltaTime;
}

void ITransform::moveByInputVector(glm::vec2 inputVector,float deltaTime) {
    transform.velocity.x = (inputVector.x * (1.0f - smooth) + transform.velocity.x * smooth) * speed;
    transform.velocity.y = (inputVector.y * (1.0f - smooth) + transform.velocity.y * smooth) * speed;
    translate(deltaTime);
}

ITransform::ITransform(glm::vec2 position, float rotation, glm::vec2 vel, glm::vec2 scale) : transform(position,rotation,vel,scale){

}

float ITransform::rotateTowardsPosition2D(const glm::vec2& targetPosition) {
    // Calculate the direction vector from the current position to the target position  
    glm::vec2 directionToTarget = glm::normalize(targetPosition - transform.position);

    // Convert the current rotation angle to a directional vector  
    glm::vec2 currentDirection = glm::vec2(cos( transform.rotation), sin( transform.rotation));

    // Calculate the angle between the current direction and the target direction  
    float angleToTarget = glm::orientedAngle(currentDirection, directionToTarget);
    
    // Return the new rotation angle 
    return transform.rotation + angleToTarget;
}

void ITransform::imgui_render() {
    ImGui::InputFloat2("Position",glm::value_ptr(transform.position));
    ImGui::InputFloat("Rotation",glm::value_ptr(transform.scale));
    ImGui::InputFloat2("Scale",glm::value_ptr(transform.scale));
}

Transform::Transform(glm::vec2 vec1, float d, glm::vec2 vec2, glm::vec2 vec3) {
position = vec1;
rotation = d;
velocity = vec2;
scale = vec3;
}
