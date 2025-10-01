#pragma once

#include "types.h"
#include <chrono>

namespace GameEngine2D {

class TimeManager {
public:
    TimeManager();
    ~TimeManager();
    
    bool initialize();
    void shutdown();
    void update();
    
    float getDeltaTime() const { return m_deltaTime; }
    float getTotalTime() const { return m_totalTime; }
    float getFPS() const { return m_fps; }
    
private:
    TimePoint m_lastTime;
    TimePoint m_startTime;
    float m_deltaTime;
    float m_totalTime;
    float m_fps;
    
    void calculateFPS();
};

} // namespace GameEngine2D
