//
// Created by redkc on 03/01/2024.
//

#include "Tile.h"

Tile::Tile(Shader *shader, Texture *texture, GLuint vao, CollsionTypeEnum collsionType, bool isKinematic,glm::vec2 position, float rotation, glm::vec2 vel, glm::vec2 scale) : IRenderable(
        shader, texture, vao), ICollision(collsionType, isKinematic,scale), ITransform(position,rotation,vel,scale) {
    vertexCount = 6;
}

void Tile::imgui_render(std::map<std::string, std::shared_ptr<Texture>> textureMap) {
    ImGui::PushID(uniqueID);
    string  name  = "Tile nr:" + std::to_string(uniqueID);
    if (ImGui::TreeNode(name.c_str())) {
        ITransform::imgui_render();
        IRenderable::imgui_select_new_texture(textureMap);
        ImGui::TreePop();
    }
    ImGui::PopID();
}
