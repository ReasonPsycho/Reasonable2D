//
// Created by redkc on 02/01/2024.
//

#include "IRenderable.h"
#include "glm/fwd.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"


void IRenderable::render() {
    
    texture->use(GL_TEXTURE0);
    shader->use();

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(transform.position(), -50));
    if(transform.rotation() != 0) 
        model = glm::rotate(model,transform.rotation() + 1.7f,glm::vec3 (0,0,1));
    model = glm::scale(model, glm::vec3(transform.scale(),1.0f));
    shader->setMatrix4("transform", false, glm::value_ptr(model));
    shader->setVec3("lightColor",255.0f, 255.0f, 255.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
}

void IRenderable::render(float z) {

    texture->use(GL_TEXTURE0);
    shader->use();

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(transform.position(), z));
    if(transform.rotation() != 0)
        model = glm::rotate(model,transform.rotation() + 1.7f,glm::vec3 (0,0,1));
    model = glm::scale(model, glm::vec3(transform.scale(),1.0f));
    shader->setMatrix4("transform", false, glm::value_ptr(model));
    shader->setVec3("lightColor",255.0f, 255.0f, 255.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
}

IRenderable::IRenderable(Shader* shader, Texture* texture,GLuint VAO): shader(shader),texture(texture),VAO(VAO) {

}

IRenderable::~IRenderable() {

}

IRenderable::IRenderable(Shader* shader, Texture* texture): shader(shader),texture(texture) {

}

void IRenderable::imgui_render() {
// Create a buffer with a maximum size of your choice.
    std::vector<char> buffer(100);

// Copy std::string to buffer
    std::copy(texture->name.begin(), texture->name.end(), buffer.begin());

// Make sure buffer is null terminated.
    buffer[texture->name.size()] = '\0';

    ImGui::InputText("Texture name", buffer.data(), buffer.size());

// After ImGui::InputText, you can copy the contents buffer data
// back into `texture->name`. Don't forget to remove trailing null characters
    texture->name = std::string(buffer.data());
}

void IRenderable::imgui_select_new_texture(std::map<std::string, std::shared_ptr<Texture>> textureMap) {
    std::string current_item = texture->name; // This must be accessible later on when rendering the combo box

// Iterating through textureMap and adding each (key, value) pair to combo box.
    if (ImGui::BeginCombo("textures", current_item.c_str())) // The second parameter is the label previewed before opening the combo.
    {
        for (auto& item : textureMap)
        {
            bool is_selected = (current_item == item.first);
            if (ImGui::Selectable(item.first.c_str(), is_selected))
                current_item = item.first;

            if (is_selected)
                ImGui::SetItemDefaultFocus(); // You may want to set the item which is being hovered as the currently selected item
        }
        ImGui::EndCombo();
    }

// The variable 'current_item' will hold the string that's currently selected when you select a new item from the combo box (drop-down list)
// You can use 'current_item' to get the selected shared_ptr<Texture> as use it as follows:
    texture = textureMap[current_item].get();
}
