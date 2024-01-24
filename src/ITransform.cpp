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
    auto diffrence =  mousePosition / 2.0f - transform.position() ;
    
    transform.setVelocity((glm::clamp(glm::vec2(-1.0f),diffrence,glm::vec2(1.0f)) * (1.0f - smooth) +  transform.velocity() * smooth) * speed);
}

void ITransform::translate(float deltaTime ) {
    transform.setPosition( transform.position() += transform.velocity()* deltaTime );
}

void ITransform::moveByInputVector(glm::vec2 inputVector,float deltaTime) {
    transform.setVelocity((inputVector * (1.0f - smooth) + transform.velocity() * smooth) * speed); 
}

ITransform::ITransform(glm::vec2 position, float rotation, glm::vec2 vel, glm::vec2 scale) : transform(position,rotation,scale){

}


void ITransform::imgui_render(Camera* camera) {
    glm::vec2 temp_position = transform.position();
    if (ImGui::InputFloat2("Position", glm::value_ptr(temp_position))) {
        transform.setPosition(temp_position);
    }

    float temp_rotation = glm::degrees(transform.rotation());
    if (ImGui::InputFloat("Rotation", &temp_rotation)) {
        temp_rotation = glm::radians(temp_rotation); // Convert degrees to radians
        transform.setRotation(temp_rotation);
    }

    glm::vec2 temp_scale = transform.scale();
    if (ImGui::InputFloat2("Scale", glm::value_ptr(temp_scale))) {
        transform.setScale(temp_scale);
    }
}
