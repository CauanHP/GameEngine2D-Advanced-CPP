#pragma once

#include "types.h"

namespace GameEngine2D {

class SceneManager {
public:
    SceneManager();
    ~SceneManager();
    
    bool initialize();
    void shutdown();
    
    void update(float deltaTime);
    void fixedUpdate(float fixedDeltaTime);
    void render();
};

} // namespace GameEngine2D
