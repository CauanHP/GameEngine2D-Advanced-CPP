#include "audio/audio_manager.h"
#include "utils/logger.h"

namespace GameEngine2D {

AudioManager::AudioManager() {
}

AudioManager::~AudioManager() {
    shutdown();
}

bool AudioManager::initialize() {
    LOG_INFO("AudioManager initialized");
    return true;
}

void AudioManager::shutdown() {
    LOG_INFO("AudioManager shutdown");
}

void AudioManager::update(float deltaTime) {
    // Audio update logic
}

} // namespace GameEngine2D
