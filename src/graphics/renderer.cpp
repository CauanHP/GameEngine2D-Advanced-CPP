#include "graphics/renderer.h"
#include "utils/logger.h"
#include <GL/glew.h>

namespace GameEngine2D {

Renderer::Renderer() {
}

Renderer::~Renderer() {
    shutdown();
}

bool Renderer::initialize() {
    // Set initial OpenGL state
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    
    // Set clear color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    
    LOG_INFO("Renderer initialized");
    return true;
}

void Renderer::shutdown() {
    LOG_INFO("Renderer shutdown");
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void Renderer::present() {
    // Present is handled by window swap buffers
}

void Renderer::setClearColor(const Color& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::enableBlending(bool enable) {
    if (enable) {
        glEnable(GL_BLEND);
    } else {
        glDisable(GL_BLEND);
    }
}

void Renderer::setBlendMode(BlendMode mode) {
    switch (mode) {
        case BlendMode::ALPHA:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        case BlendMode::ADDITIVE:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
        case BlendMode::MULTIPLY:
            glBlendFunc(GL_DST_COLOR, GL_ZERO);
            break;
        case BlendMode::SCREEN:
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
            break;
        default:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
    }
}

} // namespace GameEngine2D
