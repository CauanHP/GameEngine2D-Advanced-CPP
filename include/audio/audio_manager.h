#pragma once

#include "types.h"

namespace GameEngine2D {

class AudioManager {
public:
    AudioManager();
    ~AudioManager();
    
    bool initialize();
    void shutdown();
    
    void update(float deltaTime);
};

} // namespace GameEngine2D
