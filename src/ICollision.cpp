//
// Created by redkc on 02/01/2024.
//

#include "ICollision.h"
#include "glm/common.hpp"
#include "glm/geometric.hpp"

int ICollision::nextID = 0;

ICollision::ICollision(CollsionTypeEnum collsionType, bool isKinematic,glm::vec2 scale) : collsionType(collsionType),
                                                                          isKinematic(isKinematic) {
    if(collsionType == Square){
        setReactPoints(glm::vec4(-scale.x/2,scale.x/2,-scale.y/2,+scale.y/2) );
    }else{
        radius = scale.x/2;
    }
    
    uniqueID = nextID++; // Assign the current value of nextID and then increment it for the next instance
}

bool ICollision::detectCollisions(const std::vector<ICollision> otherObjects) {
    if (!isKinematic) { // I won't care and just u know do it for all of them XD
        glm::vec2 SumOfSeperationVector = glm::vec2(0);
        int amountOfCollisions = 0;
        for (int i = 0; i < otherObjects.size(); ++i) {
            if (&otherObjects[i] != this) {
                glm::vec2 seperationVector = glm::vec2(0);
                if (otherObjects[i].collsionType == Circle) {
                    seperationVector = CircleSeparationVector(otherObjects[i]);
                } else {
                    seperationVector = SquareSeparationVector(otherObjects[i]);
                }
                if (seperationVector != glm::vec2(0)) {
                    amountOfCollisions++;
                    SumOfSeperationVector += seperationVector;
                }
            }
        }
        if (amountOfCollisions == 0){
            meanSeparationVector = glm::vec2(0);
        }else{
            meanSeparationVector = SumOfSeperationVector / (float) amountOfCollisions;
        }
    
        return amountOfCollisions != 0;
    }
}

bool ICollision::operator==(const ICollision &c) const {
    return this->uniqueID == c.uniqueID;
}

bool ICollision::operator!=(const ICollision &c) const {
    return this->uniqueID == c.uniqueID;
}

glm::vec2 ICollision::SquareSeparationVector(ICollision object) { // Could have prob done it better but eh
    glm::vec2 separationVector = glm::vec2(0);
    if (this->collsionType == Circle) {
        glm::vec2 f = glm::vec2(
                glm::clamp(transform.position.x, object.getRelativeReactPoints().x, object.getRelativeReactPoints().y),
                glm::clamp(transform.position.y,  object.getRelativeReactPoints().z, object.getRelativeReactPoints().w));
        float length = glm::length(transform.position - f);
        if (length <= radius) {
            if (transform.position != f) {
                separationVector = ((transform.position - f) / length) * (radius - length);
            } else {
                float left = transform.position.x - object.getRelativeReactPoints().x + radius;
                float top = transform.position.y - object.getRelativeReactPoints().z + radius;
                float right = object.getRelativeReactPoints().y - transform.position.x + radius;
                float bottom = object.getRelativeReactPoints().w - transform.position.y + radius;
                separationVector = glm::vec2(glm::min(left, right), glm::min(top, bottom));
            }
        }
    } else {
        //l – left, min x-coordinate of rectangle, x
        //r – right, max x-coordinate of rectangle, y
        //t – top, min y-coordinate of rectangle (y increases downwards!), z
        //b – bottom, max y-coordinate of rectangle, w
        // r1 > l2 ∧ r2 > l1 ∧ b1 > t2 ∧ b2 > t1

        float left = getRelativeReactPoints().y - object.getRelativeReactPoints().x;
        float right = object.getRelativeReactPoints().y - getRelativeReactPoints().x;
        float top = getRelativeReactPoints().w - object.getRelativeReactPoints().z;
        float bottom = object.getRelativeReactPoints().w - getRelativeReactPoints().z;
        if (left > 0 && right > 0 && top > 0 && bottom > 0){
            if (left < right){
                separationVector.x = -left;
            } else{
                separationVector.x = right;
            }
            if (top < bottom){
                separationVector.y = -top;
            } else{
                separationVector.y = bottom;
            }
            if(abs(separationVector.x) < abs(separationVector.y)){
                separationVector.y = 0;
            } else if (abs(separationVector.x) != abs(separationVector.y)) {
                separationVector.x = 0;
            }
        }
    }
    return separationVector;
}

glm::vec2 ICollision::CircleSeparationVector(const ICollision &object) { // Could have prob done it better but eh
    if (this->collsionType == Circle) {
        float distance = abs(object.transform.position - transform.position).length();
        if (distance < radius + object.radius) {
            glm::vec2 separationVector =
                    ((transform.position - object.transform.position) / glm::length(transform.position - object.transform.position)) *
                    (radius + object.radius - glm::length(transform.position - object.transform.position));
            return separationVector;
        }
    } else {
        glm::vec2 f = glm::vec2(glm::clamp(object.transform.position.x, getRelativeReactPoints().x, getRelativeReactPoints().y),
                                glm::clamp(object.transform.position.y, getRelativeReactPoints().z, getRelativeReactPoints().w));
        if (glm::length(object.transform.position - f) >= object.radius) {
            glm::vec2 separationVector = glm::vec2(0);
            if (object.transform.position != f) {
                separationVector = ((object.transform.position - f) / glm::length(object.transform.position - f)) *
                                   (object.radius - glm::length(object.transform.position - f));
            } else {
                float left = object.transform.position.x - getRelativeReactPoints().x + object.radius;
                float top = object.transform.position.y - getRelativeReactPoints().z + object.radius;
                float right = getRelativeReactPoints().y - object.transform.position.x + object.radius;
                float bottom = getRelativeReactPoints().w - object.transform.position.y + object.radius;
                separationVector = glm::vec2(glm::min(left, right), glm::min(top, bottom));
            }
            return separationVector;
        }
    }
    return glm::vec2(0);
}

void ICollision::seperateObject() {
    if (meanSeparationVector != glm:: vec2(0)){
        this->transform.position += meanSeparationVector;
    }
}

void ICollision::bounceObject() {
    glm::vec2 normalizedSeparation = meanSeparationVector;
    this->transform.velocity += normalizedSeparation;
}

void ICollision::setReactPoints(glm::vec4 newReactPoints) {
    reactPoints = newReactPoints;
}

const glm::vec4 ICollision::getRelativeReactPoints() {
    return {reactPoints.x + transform.position.x, reactPoints.y + transform.position.x, reactPoints.z + transform.position.y,
            reactPoints.w + transform.position.y};
}

const glm::vec4 ICollision::getReactPoints() {
    return reactPoints;
}
