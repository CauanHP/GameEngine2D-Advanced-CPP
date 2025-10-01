#pragma once

#include "types.h"

namespace GameEngine2D {

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    bool initialize();
    void shutdown();
    
    void clear();
    void setViewport(int x, int y, int width, int height);
    void present();
    
    void setClearColor(const Color& color);
    void enableBlending(bool enable);
    void setBlendMode(BlendMode mode);
};

} // namespace GameEngine2D
