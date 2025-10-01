#pragma once

#include "types.h"

namespace GameEngine2D {

class PhysicsEngine {
public:
    PhysicsEngine();
    ~PhysicsEngine();
    
    bool initialize();
    void shutdown();
    
    void update(float deltaTime);
};

} // namespace GameEngine2D
