#pragma once

#include "types.h"
#include "core/window.h"
#include "core/time_manager.h"
#include "graphics/renderer.h"
#include "scene/scene_manager.h"
#include "audio/audio_manager.h"
#include "physics/physics_engine.h"
#include <memory>
#include <functional>

namespace GameEngine2D {

// Forward declarations
class Application;

// Application callbacks
using UpdateCallback = std::function<void(float)>;
using RenderCallback = std::function<void()>;
using EventCallback = std::function<void(const Event&)>;

class Application {
public:
    Application(const WindowConfig& windowConfig = WindowConfig{});
    ~Application();
    
    // Application lifecycle
    bool initialize();
    void run();
    void shutdown();
    
    // Application state
    bool isRunning() const { return m_running; }
    void stop() { m_running = false; }
    
    // Window access
    Window* getWindow() const { return m_window.get(); }
    
    // Core systems access
    TimeManager* getTimeManager() const { return m_timeManager.get(); }
    Renderer* getRenderer() const { return m_renderer.get(); }
    SceneManager* getSceneManager() const { return m_sceneManager.get(); }
    AudioManager* getAudioManager() const { return m_audioManager.get(); }
    PhysicsEngine* getPhysicsEngine() const { return m_physicsEngine.get(); }
    
    // Callbacks
    void setUpdateCallback(UpdateCallback callback) { m_updateCallback = callback; }
    void setRenderCallback(RenderCallback callback) { m_renderCallback = callback; }
    void setEventCallback(EventCallback callback) { m_eventCallback = callback; }
    
    // Application configuration
    void setTargetFPS(float fps) { m_targetFPS = fps; }
    void setFixedTimeStep(float timeStep) { m_fixedTimeStep = timeStep; }
    void enableVSync(bool enable);
    void setWindowTitle(const std::string& title);
    
    // Statistics
    float getFPS() const { return m_fps; }
    float getFrameTime() const { return m_frameTime; }
    float getDeltaTime() const { return m_deltaTime; }
    
    // Static access
    static Application* getInstance() { return s_instance; }

private:
    // Core systems
    std::unique_ptr<Window> m_window;
    std::unique_ptr<TimeManager> m_timeManager;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<SceneManager> m_sceneManager;
    std::unique_ptr<AudioManager> m_audioManager;
    std::unique_ptr<PhysicsEngine> m_physicsEngine;
    
    // Application state
    bool m_running;
    bool m_initialized;
    
    // Timing
    float m_targetFPS;
    float m_fixedTimeStep;
    float m_fps;
    float m_frameTime;
    float m_deltaTime;
    float m_accumulator;
    
    // Callbacks
    UpdateCallback m_updateCallback;
    RenderCallback m_renderCallback;
    EventCallback m_eventCallback;
    
    // Static instance
    static Application* s_instance;
    
    // Internal methods
    void initializeSystems();
    void shutdownSystems();
    void handleEvents();
    void update(float deltaTime);
    void fixedUpdate(float fixedDeltaTime);
    void render();
    void updateStatistics(float deltaTime);
    
    // Event handling
    void onWindowResize(int width, int height);
    void onKeyPress(KeyCode key, InputAction action, int mods);
    void onMouseButton(MouseButton button, InputAction action, int mods);
    void onMouseMove(double x, double y);
    void onMouseScroll(double xoffset, double yoffset);
};

} // namespace GameEngine2D
