//
// Created by redkc on 02/01/2024.
//

#ifndef OPENGLGP_IRENDERABLE_H
#define OPENGLGP_IRENDERABLE_H


#include <map>
#include "ITransform.h"
#include "Utilities/Shader.h"
#include "Utilities/Texture.h"
#include "imgui.h"

namespace ImGui
{
    // ImGui::InputText() with std::string
    // Because text input needs dynamic resizing, we need to setup a callback to grow the capacity
    IMGUI_API bool  InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
    IMGUI_API bool  InputTextMultiline(const char* label, std::string* str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
    IMGUI_API bool  InputTextWithHint(const char* label, const char* hint, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
}

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
    void render(float z);
    void render(float z,glm::vec3 color) ;
    void render(glm::vec2 pos,float z, glm::vec3 color,float scroolSpeed) ;
    void imgui_render();
    void imgui_select_new_texture(std::map<std::string, std::shared_ptr<Texture>> textureMap);
};


#endif //OPENGLGP_IRENDERABLE_H
