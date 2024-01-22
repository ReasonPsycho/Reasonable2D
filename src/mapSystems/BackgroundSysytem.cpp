//
// Created by redkc on 03/01/2024.
//

#include "BackgroundSystem.h"


void BackgroundSystem::init() {

}

BackgroundSystem::BackgroundSystem(Shader *shader,string path) : shader(shader) {
  
}

void BackgroundSystem::render() {
    for (int i = 0; i < layers.size(); i++) {
        layers[i].render();
    }
}
