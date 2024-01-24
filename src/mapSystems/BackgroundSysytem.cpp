//
// Created by redkc on 03/01/2024.
//

#include "BackgroundSystem.h"


void BackgroundSystem::init() {

}


void BackgroundSystem::render() {
    for (int i = 0; i < layers.size(); i++) {
        layers[i].render();
    }
}

void BackgroundSystem::addLayer(string path, float z, glm::vec3 color) {
    layers.push_back(BackgroundLayer(shader,path, z, color));
}
