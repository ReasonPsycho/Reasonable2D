//
// Created by redkc on 20/11/2023.
//

#include "Circlee.h"


Circlee::Circlee(Shader *shader, Texture *texture, CollsionTypeEnum collsionType, bool isKinematic,
                 glm::vec2 position, float rotation, glm::vec2 vel, glm::vec2 scale) : IRenderable(
        shader, texture), ICollision(collsionType, isKinematic, scale), ITransform(position, rotation, vel,
                                                                                        scale) {
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
    vertexCount = 100;
}
