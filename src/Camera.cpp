//
// Created by redkc on 21/11/2023.
//

#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch,float nearClip, float farClip) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM),nearClip(nearClip),farClip(farClip)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
    if (direction == UPWARD)
        Position += Up * velocity;
    if (direction == DOWNWARD)
        Position -= Up * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch, float deltaTime) {
    xoffset *= MouseSensitivity * deltaTime;
    yoffset *= MouseSensitivity * deltaTime;

    Yaw   += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset, float deltaTime) {
    Zoom -= (float)yoffset * deltaTime;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::updateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = glm::normalize(glm::cross(Right, Front));
}

void Camera::UpdateShader(Shader* shader) {
    shader->use();
    // pass projection matrix to shader 
    glm::mat4 projection = GetProjectionMatrix();
    shader->setMatrix4("projection", false, glm::value_ptr(projection));

    // camera/view transformation
    glm::mat4 view = GetViewMatrix();
    shader->setMatrix4("view", false, glm::value_ptr(view));
}

glm::mat4 Camera::GetProjectionMatrix() {
    return  glm::perspective(glm::radians(Zoom), (float)screenWidth / (float)screenHeight, nearClip, farClip);
}


glm::vec3 Camera::ScreenToWorld(glm::vec3 screenPos)
{
    glReadPixels(screenPos.x, screenHeight-screenPos.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &screenPos.z); // This is useless at this point buuuuut it's probably usefully to save that
    glm::mat4 projection = glm::perspective(glm::radians(Zoom), (float)screenWidth / (float)screenHeight, nearClip, farClip);
    glm::mat4 view = GetViewMatrix();
    glm::vec4 screenPos_ndc = glm::vec4(
            screenPos.x / (float)screenWidth * 2.0f - 1.0f,
            ((float )screenHeight - screenPos.y) / (float)screenHeight * 2.0f - 1.0f,  // flip the y axis
            2.0f * screenPos.z - 1.0f,
            1.0f
    );
    glm::mat4 inverseVP = glm::inverse(projection * view);
    glm::vec4 worldPos_homogeneous = inverseVP * screenPos_ndc;

    glm::vec3 worldPos = glm::vec3(
            worldPos_homogeneous.x / worldPos_homogeneous.w,
            worldPos_homogeneous.y / worldPos_homogeneous.w,
            worldPos_homogeneous.z / worldPos_homogeneous.w
    );

    return worldPos  ;
}

void Camera::SetUpSingleGlViewport(int width, int height) {
    glViewport(0, 0, width, height);
    screenWidth = width;
    screenHeight = height;
    widnowWidth = width;
    widnowHeight = height;
}

void Camera::SetUpSplitGlViewport(int i,int width, int height) {
    if(i == 0){
        glViewport(0, 0, width/2, height);
    }else{
        glViewport( width/2, 0, width/2, height);
        
    }
    screenWidth = width/2;
    screenHeight = height;
    widnowWidth = width;
    widnowHeight = height;
}
