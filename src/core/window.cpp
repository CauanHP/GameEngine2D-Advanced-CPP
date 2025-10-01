#include "core/window.h"
#include "utils/logger.h"
#include <GLFW/glfw3.h>
#include <algorithm>

namespace GameEngine2D {

// Static member initialization
Window* Window::s_currentContext = nullptr;

Window::Window(const WindowConfig& config) : m_config(config), m_window(nullptr) {
    m_lastMousePosition = Vector2(0, 0);
    m_mouseDelta = Vector2(0, 0);
    m_scrollDelta = Vector2(0, 0);
}

Window::~Window() {
    shutdown();
}

bool Window::initialize() {
    // Initialize GLFW if not already done
    if (!glfwInit()) {
        LOG_ERROR("Failed to initialize GLFW");
        return false;
    }
    
    // Set GLFW window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, m_config.resizable ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA
    
    // Create window
    GLFWmonitor* monitor = m_config.fullscreen ? glfwGetPrimaryMonitor() : nullptr;
    m_window = glfwCreateWindow(m_config.width, m_config.height, 
                               m_config.title.c_str(), monitor, nullptr);
    
    if (!m_window) {
        LOG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }
    
    // Set context
    glfwMakeContextCurrent(m_window);
    setCurrentContext(this);
    
    // Set callbacks
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, glfwWindowResizeCallback);
    glfwSetKeyCallback(m_window, glfwKeyCallback);
    glfwSetMouseButtonCallback(m_window, glfwMouseButtonCallback);
    glfwSetCursorPosCallback(m_window, glfwMouseMoveCallback);
    glfwSetScrollCallback(m_window, glfwMouseScrollCallback);
    glfwSetCharCallback(m_window, glfwCharCallback);
    
    // Set VSync
    glfwSwapInterval(m_config.vsync ? 1 : 0);
    
    // Center window if not fullscreen
    if (!m_config.fullscreen) {
        center();
    }
    
    LOG_INFO_FMT("Window created: {}x{} - {}", m_config.width, m_config.height, m_config.title);
    return true;
}

void Window::shutdown() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
        
        if (s_currentContext == this) {
            s_currentContext = nullptr;
        }
        
        LOG_INFO("Window destroyed");
    }
}

bool Window::shouldClose() const {
    return m_window ? glfwWindowShouldClose(m_window) : true;
}

void Window::swapBuffers() {
    if (m_window) {
        glfwSwapBuffers(m_window);
    }
}

void Window::pollEvents() {
    glfwPollEvents();
    updateInputState();
}

void Window::setTitle(const std::string& title) {
    m_config.title = title;
    if (m_window) {
        glfwSetWindowTitle(m_window, title.c_str());
    }
}

void Window::setSize(int width, int height) {
    m_config.width = width;
    m_config.height = height;
    if (m_window) {
        glfwSetWindowSize(m_window, width, height);
    }
}

void Window::setFullscreen(bool fullscreen) {
    if (m_config.fullscreen == fullscreen) return;
    
    m_config.fullscreen = fullscreen;
    
    if (m_window) {
        GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
        const GLFWvidmode* mode = fullscreen ? glfwGetVideoMode(monitor) : nullptr;
        
        if (fullscreen && mode) {
            glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        } else {
            glfwSetWindowMonitor(m_window, nullptr, 100, 100, m_config.width, m_config.height, 0);
        }
    }
}

void Window::setVSync(bool enabled) {
    m_config.vsync = enabled;
    if (m_window) {
        glfwSwapInterval(enabled ? 1 : 0);
    }
}

void Window::center() {
    if (!m_window) return;
    
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    
    int x = (mode->width - m_config.width) / 2;
    int y = (mode->height - m_config.height) / 2;
    
    glfwSetWindowPos(m_window, x, y);
}

void Window::maximize() {
    if (m_window) {
        glfwMaximizeWindow(m_window);
    }
}

void Window::minimize() {
    if (m_window) {
        glfwIconifyWindow(m_window);
    }
}

void Window::restore() {
    if (m_window) {
        glfwRestoreWindow(m_window);
    }
}

bool Window::isKeyPressed(KeyCode key) const {
    if (!m_window) return false;
    
    int glfwKey = static_cast<int>(key);
    return glfwGetKey(m_window, glfwKey) == GLFW_PRESS;
}

bool Window::isMouseButtonPressed(MouseButton button) const {
    if (!m_window) return false;
    
    int glfwButton = static_cast<int>(button);
    return glfwGetMouseButton(m_window, glfwButton) == GLFW_PRESS;
}

Vector2 Window::getMousePosition() const {
    if (!m_window) return Vector2(0, 0);
    
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);
    return Vector2(static_cast<float>(x), static_cast<float>(y));
}

Vector2 Window::getMouseDelta() const {
    return m_mouseDelta;
}

Vector2 Window::getScrollDelta() const {
    return m_scrollDelta;
}

void Window::setWindowResizeCallback(WindowResizeCallback callback) {
    m_resizeCallback = callback;
}

void Window::setKeyCallback(KeyCallback callback) {
    m_keyCallback = callback;
}

void Window::setMouseButtonCallback(MouseButtonCallback callback) {
    m_mouseButtonCallback = callback;
}

void Window::setMouseMoveCallback(MouseMoveCallback callback) {
    m_mouseMoveCallback = callback;
}

void Window::setMouseScrollCallback(MouseScrollCallback callback) {
    m_mouseScrollCallback = callback;
}

void Window::setCharCallback(CharCallback callback) {
    m_charCallback = callback;
}

void Window::setCurrentContext(Window* window) {
    s_currentContext = window;
}

Window* Window::getCurrentContext() {
    return s_currentContext;
}

// GLFW callback implementations
void Window::glfwWindowResizeCallback(GLFWwindow* window, int width, int height) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        win->m_config.width = width;
        win->m_config.height = height;
        
        if (win->m_resizeCallback) {
            win->m_resizeCallback(width, height);
        }
        
        LOG_DEBUG_FMT("Window resized to {}x{}", width, height);
    }
}

void Window::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win && win->m_keyCallback) {
        KeyCode keyCode = win->glfwToKeyCode(key);
        InputAction inputAction = win->glfwToInputAction(action);
        win->m_keyCallback(keyCode, inputAction, mods);
    }
}

void Window::glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win && win->m_mouseButtonCallback) {
        MouseButton mouseButton = win->glfwToMouseButton(button);
        InputAction inputAction = win->glfwToInputAction(action);
        win->m_mouseButtonCallback(mouseButton, inputAction, mods);
    }
}

void Window::glfwMouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        Vector2 currentPos(static_cast<float>(xpos), static_cast<float>(ypos));
        win->m_mouseDelta = currentPos - win->m_lastMousePosition;
        win->m_lastMousePosition = currentPos;
        
        if (win->m_mouseMoveCallback) {
            win->m_mouseMoveCallback(xpos, ypos);
        }
    }
}

void Window::glfwMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        win->m_scrollDelta = Vector2(static_cast<float>(xoffset), static_cast<float>(yoffset));
        
        if (win->m_mouseScrollCallback) {
            win->m_mouseScrollCallback(xoffset, yoffset);
        }
    }
}

void Window::glfwCharCallback(GLFWwindow* window, unsigned int codepoint) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win && win->m_charCallback) {
        win->m_charCallback(codepoint);
    }
}

void Window::updateInputState() {
    // Reset delta values
    m_mouseDelta = Vector2(0, 0);
    m_scrollDelta = Vector2(0, 0);
}

KeyCode Window::glfwToKeyCode(int glfwKey) const {
    // Map GLFW key codes to our KeyCode enum
    if (glfwKey >= GLFW_KEY_A && glfwKey <= GLFW_KEY_Z) {
        return static_cast<KeyCode>(glfwKey);
    }
    if (glfwKey >= GLFW_KEY_0 && glfwKey <= GLFW_KEY_9) {
        return static_cast<KeyCode>(glfwKey);
    }
    if (glfwKey >= GLFW_KEY_F1 && glfwKey <= GLFW_KEY_F12) {
        return static_cast<KeyCode>(glfwKey);
    }
    
    // Special keys
    switch (glfwKey) {
        case GLFW_KEY_SPACE: return KeyCode::SPACE;
        case GLFW_KEY_APOSTROPHE: return KeyCode::APOSTROPHE;
        case GLFW_KEY_COMMA: return KeyCode::COMMA;
        case GLFW_KEY_MINUS: return KeyCode::MINUS;
        case GLFW_KEY_PERIOD: return KeyCode::PERIOD;
        case GLFW_KEY_SLASH: return KeyCode::SLASH;
        case GLFW_KEY_SEMICOLON: return KeyCode::SEMICOLON;
        case GLFW_KEY_EQUAL: return KeyCode::EQUAL;
        case GLFW_KEY_LEFT_BRACKET: return KeyCode::LEFT_BRACKET;
        case GLFW_KEY_BACKSLASH: return KeyCode::BACKSLASH;
        case GLFW_KEY_RIGHT_BRACKET: return KeyCode::RIGHT_BRACKET;
        case GLFW_KEY_GRAVE_ACCENT: return KeyCode::GRAVE_ACCENT;
        case GLFW_KEY_ESCAPE: return KeyCode::ESCAPE;
        case GLFW_KEY_ENTER: return KeyCode::ENTER;
        case GLFW_KEY_TAB: return KeyCode::TAB;
        case GLFW_KEY_BACKSPACE: return KeyCode::BACKSPACE;
        case GLFW_KEY_INSERT: return KeyCode::INSERT;
        case GLFW_KEY_DELETE: return KeyCode::DELETE;
        case GLFW_KEY_RIGHT: return KeyCode::RIGHT;
        case GLFW_KEY_LEFT: return KeyCode::LEFT;
        case GLFW_KEY_DOWN: return KeyCode::DOWN;
        case GLFW_KEY_UP: return KeyCode::UP;
        case GLFW_KEY_PAGE_UP: return KeyCode::PAGE_UP;
        case GLFW_KEY_PAGE_DOWN: return KeyCode::PAGE_DOWN;
        case GLFW_KEY_HOME: return KeyCode::HOME;
        case GLFW_KEY_END: return KeyCode::END;
        case GLFW_KEY_CAPS_LOCK: return KeyCode::CAPS_LOCK;
        case GLFW_KEY_SCROLL_LOCK: return KeyCode::SCROLL_LOCK;
        case GLFW_KEY_NUM_LOCK: return KeyCode::NUM_LOCK;
        case GLFW_KEY_PRINT_SCREEN: return KeyCode::PRINT_SCREEN;
        case GLFW_KEY_PAUSE: return KeyCode::PAUSE;
        case GLFW_KEY_LEFT_SHIFT: return KeyCode::LEFT_SHIFT;
        case GLFW_KEY_LEFT_CONTROL: return KeyCode::LEFT_CONTROL;
        case GLFW_KEY_LEFT_ALT: return KeyCode::LEFT_ALT;
        case GLFW_KEY_LEFT_SUPER: return KeyCode::LEFT_SUPER;
        case GLFW_KEY_RIGHT_SHIFT: return KeyCode::RIGHT_SHIFT;
        case GLFW_KEY_RIGHT_CONTROL: return KeyCode::RIGHT_CONTROL;
        case GLFW_KEY_RIGHT_ALT: return KeyCode::RIGHT_ALT;
        case GLFW_KEY_RIGHT_SUPER: return KeyCode::RIGHT_SUPER;
        case GLFW_KEY_MENU: return KeyCode::MENU;
        default: return KeyCode::UNKNOWN;
    }
}

MouseButton Window::glfwToMouseButton(int glfwButton) const {
    return static_cast<MouseButton>(glfwButton);
}

InputAction Window::glfwToInputAction(int glfwAction) const {
    return static_cast<InputAction>(glfwAction);
}

} // namespace GameEngine2D
