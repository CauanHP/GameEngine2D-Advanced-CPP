#pragma once

#include "types.h"
#include <GLFW/glfw3.h>
#include <functional>
#include <memory>

namespace GameEngine2D {

// Forward declarations
class Application;

// Window event callbacks
using WindowResizeCallback = std::function<void(int, int)>;
using KeyCallback = std::function<void(KeyCode, InputAction, int)>;
using MouseButtonCallback = std::function<void(MouseButton, InputAction, int)>;
using MouseMoveCallback = std::function<void(double, double)>;
using MouseScrollCallback = std::function<void(double, double)>;
using CharCallback = std::function<void(unsigned int)>;

class Window {
public:
    Window(const WindowConfig& config);
    ~Window();
    
    // Window lifecycle
    bool initialize();
    void shutdown();
    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();
    
    // Window properties
    int getWidth() const { return m_config.width; }
    int getHeight() const { return m_config.height; }
    float getAspectRatio() const { return static_cast<float>(m_config.width) / m_config.height; }
    const std::string& getTitle() const { return m_config.title; }
    bool isFullscreen() const { return m_config.fullscreen; }
    bool isVSyncEnabled() const { return m_config.vsync; }
    
    // Window operations
    void setTitle(const std::string& title);
    void setSize(int width, int height);
    void setFullscreen(bool fullscreen);
    void setVSync(bool enabled);
    void center();
    void maximize();
    void minimize();
    void restore();
    
    // Input state
    bool isKeyPressed(KeyCode key) const;
    bool isMouseButtonPressed(MouseButton button) const;
    Vector2 getMousePosition() const;
    Vector2 getMouseDelta() const;
    Vector2 getScrollDelta() const;
    
    // Event callbacks
    void setWindowResizeCallback(WindowResizeCallback callback);
    void setKeyCallback(KeyCallback callback);
    void setMouseButtonCallback(MouseButtonCallback callback);
    void setMouseMoveCallback(MouseMoveCallback callback);
    void setMouseScrollCallback(MouseScrollCallback callback);
    void setCharCallback(CharCallback callback);
    
    // GLFW window access
    GLFWwindow* getGLFWWindow() const { return m_window; }
    
    // Static methods
    static void setCurrentContext(Window* window);
    static Window* getCurrentContext();

private:
    WindowConfig m_config;
    GLFWwindow* m_window;
    
    // Input state
    mutable Vector2 m_lastMousePosition;
    mutable Vector2 m_mouseDelta;
    mutable Vector2 m_scrollDelta;
    
    // Event callbacks
    WindowResizeCallback m_resizeCallback;
    KeyCallback m_keyCallback;
    MouseButtonCallback m_mouseButtonCallback;
    MouseMoveCallback m_mouseMoveCallback;
    MouseScrollCallback m_mouseScrollCallback;
    CharCallback m_charCallback;
    
    // GLFW callback functions
    static void glfwWindowResizeCallback(GLFWwindow* window, int width, int height);
    static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void glfwMouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
    static void glfwMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void glfwCharCallback(GLFWwindow* window, unsigned int codepoint);
    
    // Helper methods
    void updateInputState();
    KeyCode glfwToKeyCode(int glfwKey) const;
    MouseButton glfwToMouseButton(int glfwButton) const;
    InputAction glfwToInputAction(int glfwAction) const;
    
    // Static context management
    static Window* s_currentContext;
};

} // namespace GameEngine2D
