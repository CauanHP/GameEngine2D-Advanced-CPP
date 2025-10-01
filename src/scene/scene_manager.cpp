#include "scene/scene_manager.h"
#include "utils/logger.h"

namespace GameEngine2D {

SceneManager::SceneManager() {
}

SceneManager::~SceneManager() {
    shutdown();
}

bool SceneManager::initialize() {
    LOG_INFO("SceneManager initialized");
    return true;
}

void SceneManager::shutdown() {
    LOG_INFO("SceneManager shutdown");
}

void SceneManager::update(float deltaTime) {
    // Scene update logic
}

void SceneManager::fixedUpdate(float fixedDeltaTime) {
    // Fixed timestep scene update logic
}

void SceneManager::render() {
    // Scene rendering logic
}

} // namespace GameEngine2D
