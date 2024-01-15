//
// Created by redkc on 21/11/2023.
//

#ifndef OPENGLGP_CAMERA_H
#define OPENGLGP_CAMERA_H

#include "glm/vec3.hpp"
#include "glm/trigonometric.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Utilities/Shader.h"

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UPWARD,
    DOWNWARD
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  5.0f;
const float ZOOM        =  45.0f;



// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    //Constructors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // Set up viewports
    void  SetUpSingleGlViewport(int width,int height);
    void  SetUpSplitGlViewport(int i,int width,int height);
    
    // Returns view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();
    glm::vec3 ScreenToWorld(glm::vec3 screenPos);
    
    //InputProcessing added delta time with predefintion of one (for cases where it takes to long to implement it)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime= 1);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true,float deltaTime = 1);
    void ProcessMouseScroll(float yoffset, float deltaTime = 1);
    
    //updates shader values
    void UpdateShader(Shader* shader);
private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
    int screenWidth;
    int screenHeight;
    int widnowWidth;
    int widnowHeight;
};


#endif //OPENGLGP_CAMERA_H
