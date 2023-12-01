//
// Created by redkc on 29/11/2023.
//

#include "Crircle.h"

Crircle::Crircle(glm::vec2 pos, glm::vec2 vel) {
    position = pos;
    velocity = vel;
}


void Crircle::init() {

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    const int numSegments = 100; // Number of segments in the circle
    float radius = 0.5f; // Radius of the circle
    float circleVertices[(numSegments + 1) * 5]; // 5 values per vertex (x, y, z, s, t)

// Calculate the positions and texture coordinates of the circleVertices
    for (int i = 0; i <= numSegments; i++) {
        float angle = 2.0f * 3.14f * i / numSegments;
        circleVertices[i * 5] = radius * cos(angle); // x coordinate
        circleVertices[i * 5 + 1] = radius * sin(angle); // y coordinate
        circleVertices[i * 5 + 2] = 0.0f; // z coordinate
        circleVertices[i * 5 + 3] = cos(angle) * 0.5f + 0.5f; // s texture coordinate
        circleVertices[i * 5 + 4] = sin(angle) * 0.5f + 0.5f; // t texture coordinate


    }

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Crircle::init(GLuint newVBO, GLuint newVAO, GLuint newEBO) {
    VBO = newVBO;
    VAO = newVAO;
    EBO = newEBO;
}



void Crircle::render(Shader *shader, Texture *texture) {
    texture->use();
    shader->use(); //Don't need this yet tbh
    // creates a 4x4 matrix and makes all elements 0.0f
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(position, -30.0f));

    shader->setMatrix4("transform", false, glm::value_ptr(model));
    shader->setVec3("lightColor", 1.0f, 0.0f, 1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 100);
}

void Crircle::detectCollisions(const std::vector<Crircle> &circles) {
    collided.clear();

    for (const auto &circle: circles) {
        glm::vec2 diff = position - circle.position;
        float distance = glm::length(diff);

        if (distance < 1 + 1) {
            // Circles are colliding
            collided.push_back(circle);
        }
    }
}

bool Crircle::checkCollisionCircleRectangle(glm::vec4 reactPoints) {
    bool r = false;
    if(position.x < reactPoints.x){
        velocity.x = -velocity.x;
        r= true;
    }

    if(position.x > reactPoints.y){
        velocity.x = -velocity.x;
        r= true;
        
    }

    if(position.y < reactPoints.z){
        velocity.y = -velocity.y;
        r= true;
        
    }

    if(position.y > reactPoints.w){
        velocity.y = -velocity.y;
        r= true;
        
    }

    // If the distance is less than the circle's radius, an intersection occurs
    return r;
}

void Crircle::move() {
    position += velocity;// * dt;
}

void Crircle::collide(bool doesBallsCollideWithEachOther,const std::vector<Crircle> &circles) {
    if(checkCollisionCircleRectangle(glm::vec4(-4,4,-4,4 ))){
        return;
    }else{
        detectCollisions(circles);
        if(!circles.empty()){
        }
    }
}

Crircle::~Crircle() {

}

