//
// Created by redkc on 02/01/2024.
//

#ifndef OPENGLGP_ICOLLISION_H
#define OPENGLGP_ICOLLISION_H


#include <vector>
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "ITransform.h"

enum CollsionTypeEnum{
    Square,
    Circle
};

class ICollision : public ITransform {
public:
    ICollision(CollsionTypeEnum collsionType, bool isKinematic);
    void detectCollisions(const std::vector<ICollision> otherObjects);
    void seperateObject();
    void bounceObject();
    CollsionTypeEnum collsionType;

    bool operator==(const ICollision& c) const;
    bool operator!=(const ICollision& c) const;

    float radius;
    void setReactPoints(glm::vec4 newReactPoints);
    const glm::vec4 getRelativeReactPoints();
    const glm::vec4 getReactPoints();
    bool isKinematic; //Not affected by stuff
private:
    glm::vec4 reactPoints;
    glm::vec2  meanSeparationVector;
    static int nextID; // Static variable to keep track of the next available ID
    int uniqueID;      // Instance variable to store the unique ID for each object
    glm::vec2 SquareSeparationVector(ICollision object);
    glm::vec2 CircleSeparationVector(const ICollision &object);
};


#endif //OPENGLGP_ICOLLISION_H
