#include "core/time_manager.h"
#include "utils/logger.h"

namespace GameEngine2D {

TimeManager::TimeManager() : m_deltaTime(0.0f), m_totalTime(0.0f), m_fps(0.0f) {
}

TimeManager::~TimeManager() {
    shutdown();
}

bool TimeManager::initialize() {
    m_startTime = std::chrono::high_resolution_clock::now();
    m_lastTime = m_startTime;
    LOG_INFO("TimeManager initialized");
    return true;
}

void TimeManager::shutdown() {
    LOG_INFO("TimeManager shutdown");
}

void TimeManager::update() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    
    auto deltaDuration = currentTime - m_lastTime;
    m_deltaTime = std::chrono::duration<float>(deltaDuration).count();
    
    auto totalDuration = currentTime - m_startTime;
    m_totalTime = std::chrono::duration<float>(totalDuration).count();
    
    m_lastTime = currentTime;
    
    calculateFPS();
}

void TimeManager::calculateFPS() {
    static float fpsAccumulator = 0.0f;
    static int frameCount = 0;
    
    fpsAccumulator += m_deltaTime;
    frameCount++;
    
    if (fpsAccumulator >= 1.0f) {
        m_fps = static_cast<float>(frameCount) / fpsAccumulator;
        fpsAccumulator = 0.0f;
        frameCount = 0;
    }
}

} // namespace GameEngine2D
