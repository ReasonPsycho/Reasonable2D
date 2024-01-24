//
// Created by redkc on 03/01/2024.
//

#include "Tile.h"

Tile::Tile(Shader *shader, Texture *texture, GLuint vao, CollsionTypeEnum collsionType, bool isKinematic,glm::vec2 position, float rotation, glm::vec2 scale, glm::vec2 vel) : IRenderable(
        shader, texture, vao), ICollision(collsionType, isKinematic,scale), ITransform(position,rotation,scale,vel) {
    vertexCount = 6;
}

void Tile::imgui_render(std::map<std::string, std::shared_ptr<Texture>> textureMap,Camera* camera) {
    ImGui::PushID(uniqueID);
    string  name  = "Tile nr:" + std::to_string(uniqueID);
    // TreeNode takes the current state 'isTreeOpen' as a parameter
    if (ImGui::TreeNodeEx(name.c_str(), isNodeOpen ? ImGuiTreeNodeFlags_DefaultOpen : 0)) {
        ITransform::imgui_render(camera);
        IRenderable::imgui_select_new_texture(textureMap);
        ImGui::TreePop();
    }
    ImGui::PopID();
}