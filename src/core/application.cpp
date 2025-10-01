#include "core/application.h"
#include "utils/logger.h"
#include <GL/glew.h>
#include <algorithm>

namespace GameEngine2D {

// Static member initialization
Application* Application::s_instance = nullptr;

Application::Application(const WindowConfig& windowConfig)
    : m_running(false), m_initialized(false), m_targetFPS(60.0f), m_fixedTimeStep(1.0f / 60.0f),
      m_fps(0.0f), m_frameTime(0.0f), m_deltaTime(0.0f), m_accumulator(0.0f) {
    
    s_instance = this;
    
    // Create window
    m_window = std::make_unique<Window>(windowConfig);
    
    // Create core systems
    m_timeManager = std::make_unique<TimeManager>();
    m_renderer = std::make_unique<Renderer>();
    m_sceneManager = std::make_unique<SceneManager>();
    m_audioManager = std::make_unique<AudioManager>();
    m_physicsEngine = std::make_unique<PhysicsEngine>();
    
    LOG_INFO("Application created");
}

Application::~Application() {
    shutdown();
    s_instance = nullptr;
    LOG_INFO("Application destroyed");
}

bool Application::initialize() {
    if (m_initialized) {
        LOG_WARNING("Application already initialized");
        return true;
    }
    
    try {
        // Initialize window
        if (!m_window->initialize()) {
            LOG_ERROR("Failed to initialize window");
            return false;
        }
        
        // Initialize GLEW
        GLenum glewError = glewInit();
        if (glewError != GLEW_OK) {
            LOG_ERROR_FMT("Failed to initialize GLEW: {}", reinterpret_cast<const char*>(glewGetErrorString(glewError)));
            return false;
        }
        
        // Set up window callbacks
        m_window->setWindowResizeCallback([this](int width, int height) {
            onWindowResize(width, height);
        });
        
        m_window->setKeyCallback([this](KeyCode key, InputAction action, int mods) {
            onKeyPress(key, action, mods);
        });
        
        m_window->setMouseButtonCallback([this](MouseButton button, InputAction action, int mods) {
            onMouseButton(button, action, mods);
        });
        
        m_window->setMouseMoveCallback([this](double x, double y) {
            onMouseMove(x, y);
        });
        
        m_window->setMouseScrollCallback([this](double xoffset, double yoffset) {
            onMouseScroll(xoffset, yoffset);
        });
        
        // Initialize systems
        initializeSystems();
        
        m_initialized = true;
        m_running = true;
        
        LOG_INFO("Application initialized successfully");
        return true;
        
    } catch (const std::exception& e) {
        LOG_ERROR_FMT("Failed to initialize application: {}", e.what());
        return false;
    }
}

void Application::run() {
    if (!m_initialized) {
        LOG_ERROR("Application not initialized");
        return;
    }
    
    LOG_INFO("Starting application main loop");
    
    while (m_running && !m_window->shouldClose()) {
        // Calculate delta time
        m_deltaTime = m_timeManager->getDeltaTime();
        m_accumulator += m_deltaTime;
        
        // Handle events
        handleEvents();
        
        // Fixed timestep updates
        while (m_accumulator >= m_fixedTimeStep) {
            fixedUpdate(m_fixedTimeStep);
            m_accumulator -= m_fixedTimeStep;
        }
        
        // Variable timestep update
        update(m_deltaTime);
        
        // Render
        render();
        
        // Swap buffers and poll events
        m_window->swapBuffers();
        m_window->pollEvents();
        
        // Update statistics
        updateStatistics(m_deltaTime);
    }
    
    LOG_INFO("Application main loop ended");
}

void Application::shutdown() {
    if (!m_initialized) {
        return;
    }
    
    LOG_INFO("Shutting down application");
    
    m_running = false;
    
    // Shutdown systems
    shutdownSystems();
    
    // Shutdown window
    if (m_window) {
        m_window->shutdown();
    }
    
    m_initialized = false;
    LOG_INFO("Application shutdown completed");
}

void Application::enableVSync(bool enable) {
    if (m_window) {
        m_window->setVSync(enable);
    }
}

void Application::setWindowTitle(const std::string& title) {
    if (m_window) {
        m_window->setTitle(title);
    }
}

void Application::initializeSystems() {
    // Initialize time manager
    if (!m_timeManager->initialize()) {
        LOG_ERROR("Failed to initialize time manager");
        throw std::runtime_error("Time manager initialization failed");
    }
    
    // Initialize renderer
    if (!m_renderer->initialize()) {
        LOG_ERROR("Failed to initialize renderer");
        throw std::runtime_error("Renderer initialization failed");
    }
    
    // Initialize scene manager
    if (!m_sceneManager->initialize()) {
        LOG_ERROR("Failed to initialize scene manager");
        throw std::runtime_error("Scene manager initialization failed");
    }
    
    // Initialize audio manager
    if (!m_audioManager->initialize()) {
        LOG_ERROR("Failed to initialize audio manager");
        throw std::runtime_error("Audio manager initialization failed");
    }
    
    // Initialize physics engine
    if (!m_physicsEngine->initialize()) {
        LOG_ERROR("Failed to initialize physics engine");
        throw std::runtime_error("Physics engine initialization failed");
    }
    
    LOG_INFO("All systems initialized successfully");
}

void Application::shutdownSystems() {
    if (m_physicsEngine) {
        m_physicsEngine->shutdown();
    }
    
    if (m_audioManager) {
        m_audioManager->shutdown();
    }
    
    if (m_sceneManager) {
        m_sceneManager->shutdown();
    }
    
    if (m_renderer) {
        m_renderer->shutdown();
    }
    
    if (m_timeManager) {
        m_timeManager->shutdown();
    }
    
    LOG_INFO("All systems shutdown");
}

void Application::handleEvents() {
    // Events are handled by GLFW callbacks
    // This method can be used for custom event processing
}

void Application::update(float deltaTime) {
    // Update time manager
    m_timeManager->update();
    
    // Update scene manager
    m_sceneManager->update(deltaTime);
    
    // Update audio manager
    m_audioManager->update(deltaTime);
    
    // Call user update callback
    if (m_updateCallback) {
        m_updateCallback(deltaTime);
    }
}

void Application::fixedUpdate(float fixedDeltaTime) {
    // Update physics engine with fixed timestep
    m_physicsEngine->update(fixedDeltaTime);
    
    // Update scene manager with fixed timestep
    m_sceneManager->fixedUpdate(fixedDeltaTime);
}

void Application::render() {
    // Clear screen
    m_renderer->clear();
    
    // Render current scene
    m_sceneManager->render();
    
    // Call user render callback
    if (m_renderCallback) {
        m_renderCallback();
    }
    
    // Present frame
    m_renderer->present();
}

void Application::updateStatistics(float deltaTime) {
    static float frameTimeAccumulator = 0.0f;
    static int frameCount = 0;
    
    frameTimeAccumulator += deltaTime;
    frameCount++;
    
    if (frameTimeAccumulator >= 1.0f) {
        m_fps = static_cast<float>(frameCount) / frameTimeAccumulator;
        m_frameTime = frameTimeAccumulator / frameCount;
        
        frameTimeAccumulator = 0.0f;
        frameCount = 0;
    }
}

void Application::onWindowResize(int width, int height) {
    if (m_renderer) {
        m_renderer->setViewport(0, 0, width, height);
    }
    
    LOG_DEBUG_FMT("Window resized to {}x{}", width, height);
}

void Application::onKeyPress(KeyCode key, InputAction action, int mods) {
    if (action == InputAction::PRESS) {
        if (key == KeyCode::ESCAPE) {
            stop();
        }
    }
    
    // Create and dispatch event
    Event event;
    event.id = static_cast<EventID>(key);
    event.name = "key_press";
    event.data = &key;
    event.dataSize = sizeof(KeyCode);
    
    if (m_eventCallback) {
        m_eventCallback(event);
    }
}

void Application::onMouseButton(MouseButton button, InputAction action, int mods) {
    // Create and dispatch event
    Event event;
    event.id = static_cast<EventID>(button);
    event.name = "mouse_button";
    event.data = &button;
    event.dataSize = sizeof(MouseButton);
    
    if (m_eventCallback) {
        m_eventCallback(event);
    }
}

void Application::onMouseMove(double x, double y) {
    // Create and dispatch event
    Event event;
    event.id = 0; // Mouse move doesn't have a specific ID
    event.name = "mouse_move";
    Vector2 position(static_cast<float>(x), static_cast<float>(y));
    event.data = &position;
    event.dataSize = sizeof(Vector2);
    
    if (m_eventCallback) {
        m_eventCallback(event);
    }
}

void Application::onMouseScroll(double xoffset, double yoffset) {
    // Create and dispatch event
    Event event;
    event.id = 0; // Mouse scroll doesn't have a specific ID
    event.name = "mouse_scroll";
    Vector2 scroll(static_cast<float>(xoffset), static_cast<float>(yoffset));
    event.data = &scroll;
    event.dataSize = sizeof(Vector2);
    
    if (m_eventCallback) {
        m_eventCallback(event);
    }
}

} // namespace GameEngine2D
