//
// Created by redkc on 03/01/2024.
//

#include "BackgroundSystem.h"


void BackgroundSystem::init() {

}


void BackgroundSystem::addLayer(string path, float z, glm::vec3 color, float scroolSpeed) {
    layers.push_back(BackgroundLayer(shader, path, z, color, scroolSpeed));
}

void BackgroundSystem::imgui_render() {


    ImGui::Begin("Layer editor");

    if (ImGui::TreeNode("Layers")) {

        if (!layers.empty()) {
            for (auto &layer: layers) {
                layer.imgui_render();
            }
        };
        ImGui::TreePop();
    }
    ImGui::End();

}
