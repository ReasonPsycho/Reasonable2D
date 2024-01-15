//
// Created by redkc on 20/11/2023.
//

#include "Squere.h"
#include "Utilities/Shader.h"
#include "Utilities/Texture.h"
Squere::Squere(Shader *shader, Texture *texture, CollsionTypeEnum collsionType, bool isKinematic,glm::vec2 position, float rotation, glm::vec2 vel, glm::vec2 scale) : IRenderable(
        shader, texture), ICollision(collsionType, isKinematic,scale), ITransform(position,rotation,vel,scale) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Set up vertex data
    float vertices[] = {
            // Positions         // Texture coordinates
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VBO and EBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Specify vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    vertexCount = 6;
}
