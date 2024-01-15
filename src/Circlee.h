//
// Created by redkc on 20/11/2023.
//

#ifndef OPENGLGP_CIRCLEE_H
#define OPENGLGP_CIRCLEE_H

#include "Utilities/Shader.h"
#include "Utilities/Texture.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "ICollision.h"
#include "IRenderable.h"

class Circlee : public ICollision, public IRenderable, public virtual ITransform{ // A class to hold cube (to test concept)
public:
    Circlee(Shader *shader, Texture *texture, CollsionTypeEnum collsionType, bool isKinematic,glm::vec2 position, float rotation, glm::vec2 vel, glm::vec2 scale);
    ~Circlee();
};


#endif //OPENGLGP_SQUERE_H
