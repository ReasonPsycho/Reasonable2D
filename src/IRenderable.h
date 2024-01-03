//
// Created by redkc on 02/01/2024.
//

#ifndef OPENGLGP_IRENDERABLE_H
#define OPENGLGP_IRENDERABLE_H


#include "ITransform.h"
#include "Utilities/Shader.h"
#include "Utilities/Texture.h"

#include <glbinding/gl/gl.h>
using namespace gl;


class IRenderable : public ITransform{
public:
    IRenderable(Shader* shader, Texture* texture,GLuint VAO) ;
    ~IRenderable();
    Shader *shader;
    Texture *texture;
    GLuint VAO;
    GLuint FBO;
    GLuint EBO;
    float opacity;
    
    void render();
};


#endif //OPENGLGP_IRENDERABLE_H
