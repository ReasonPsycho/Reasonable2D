//
// Created by redkc on 02/01/2024.
//

#ifndef OPENGLGP_IRENDERABLE_H
#define OPENGLGP_IRENDERABLE_H


#include "ITransform.h"
#include "Utilities/Shader.h"
#include "Utilities/Texture.h"



class IRenderable : virtual public ITransform{
public:
    IRenderable(Shader* shader, Texture* texture,GLuint VAO) ;
    IRenderable(Shader* shader, Texture* texture) ;
    ~IRenderable();
    Shader *shader;
    Texture *texture;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    float opacity;
    int vertexCount;
    void render();
};


#endif //OPENGLGP_IRENDERABLE_H
