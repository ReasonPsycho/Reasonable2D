//
// Created by redkc on 29/11/2023.
//

#include "Crircle.h"

Crircle::Crircle(glm::vec2 pos, glm::vec2 vel, float rad, float speed) : position(pos), velocity(vel), radius(rad),
                                                                         speed(speed) {
    uniqueID = nextID++; // Assign the current value of nextID and then increment it for the next instance
}


void Crircle::init() {

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    const int numSegments = 100; // Number of segments in the circle
    float createRadius = 0.5f; // Radius of the circle
    float circleVertices[(numSegments + 1) * 5]; // 5 values per vertex (x, y, z, s, t)

// Calculate the positions and texture coordinates of the circleVertices
    for (int i = 0; i <= numSegments; i++) {
        float angle = 2.0f * 3.14f * i / numSegments;
        circleVertices[i * 5] = createRadius * cos(angle); // x coordinate
        circleVertices[i * 5 + 1] = createRadius * sin(angle); // y coordinate
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
    model = glm::scale(model, glm::vec3(radius * 2));
    shader->setMatrix4("transform", false, glm::value_ptr(model));
    shader->setVec3("lightColor", 1.0f, 0.0f, 1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 100);
}

int Crircle::detectCollisions(std::vector<Crircle> &circles, int thisCricleIndex, bool doesSeperate,
                              bool doesBounce) {

    int colisionSize = 0;
    if (doesSeperate && doesBounce) {

        for (int i = 0; i < circles.size(); ++i) {
            if (i != thisCricleIndex) {
                glm::vec2 diff = position - circles[i].position;
                float distance = glm::length(diff);

                if (distance < radius + circles[i].radius) {
                    // Circles are colliding
                    collided.push_back(circles[i]);
                }
            }
        }
        if (!collided.empty()) {
            meanSeparationVector = glm::vec2(0);
            for (int i = 0; i < collided.size(); ++i) {
                glm::vec2 separationVector =
                        ((position - collided[i].position) / glm::length(position - collided[i].position)) *
                        (radius + collided[i].radius - glm::length(position - collided[i].position));
                meanSeparationVector += separationVector;
                /*
                 if (doesSeperate) {
                    collided[i].position -= separationVector;
                }
                if (doesBounce) {
                    collided[i].velocity = -glm::normalize(separationVector);
                }
                collided[i].colided = true;
                 */
            }
            meanSeparationVector = meanSeparationVector / glm::vec2(collided.size());
            colisionSize = collided.size() + 1;
        }
    }
    collided.clear();
    return colisionSize;
}

bool Crircle::checkCollisionCircleRectangle(glm::vec4 reactPoints) {
    bool r = false;

    if (position.x - radius < reactPoints.x) {
        //position.x = -position.x + reactPoints.x + radius;
        velocity.x = glm::abs(velocity.x);
        r = true;
    }

    if (position.x + radius > reactPoints.y) {
        //position.x = -position.x + reactPoints.y - radius;
        velocity.x = glm::abs(velocity.x);
        r = true;

    }

    if (position.y - radius < reactPoints.z) {

        //position.y = -position.y + reactPoints.z - radius;
        velocity.y = glm::abs(velocity.y);
        r = true;

    }

    if (position.y + radius > reactPoints.w) {
        //position.y = -position.y + reactPoints.w + radius;
        velocity.y = glm::abs(velocity.y);
        r = true;
    }

    // If the distance is less than the circle's radius, an intersection occurs
    return r;
}

void Crircle::move(float deltaTime) {
    position += velocity * speed * deltaTime;
}

bool Crircle::collide(std::vector<Crircle> &circles, int thisCricleIndex, bool doesBallsCollideWithBalls,
                      bool doesSeperateFromBalls, bool doesBallsBounceWithWalls) {
    if (!circles.empty()) {
        return detectCollisions(circles, thisCricleIndex, doesSeperateFromBalls, doesBallsBounceWithWalls);
    }
    return false;
}

Crircle::~Crircle() {

}

bool Crircle::operator==(const Crircle &c) const {
    return this->uniqueID == c.uniqueID;
}

int Crircle::nextID = 0;

void Crircle::changeMovment(bool doesSeperate, bool doesBounce) {

    if (meanSeparationVector != glm::vec2(0)) {
        if (doesSeperate) {
            this->position += meanSeparationVector * glm::vec2(0.5);
        }
        if (doesBounce) {
            this->velocity = glm::normalize(meanSeparationVector) / glm::vec2(glm::pow(radius,2));
        }
    }
    checkCollisionCircleRectangle(glm::vec4(-10, 10, -10, 10));
}
// Initializing the static variable outside the class definition