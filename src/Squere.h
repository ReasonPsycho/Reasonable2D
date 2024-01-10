//
// Created by redkc on 20/11/2023.
//

#ifndef OPENGLGP_SQUERE_H
#define OPENGLGP_SQUERE_H



#include "Utilities/Shader.h"
#include "Utilities/Texture.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "ICollision.h"
#include "IRenderable.h"

using namespace gl;

class Squere : public ICollision, public IRenderable, public virtual ITransform{ // A class to hold cube (to test concept)
public:
    Squere(Shader *shader, Texture *texture, GLuint vao, CollsionTypeEnum collsionType, bool isKinematic,glm::vec2 position, float rotation, glm::vec2 vel, glm::vec2 scale);
    ~Squere();
};


#endif //OPENGLGP_SQUERE_H
