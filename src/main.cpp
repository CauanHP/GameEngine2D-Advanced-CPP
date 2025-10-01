#include "core/application.h"
#include "utils/logger.h"
#include <iostream>
#include <memory>

using namespace GameEngine2D;

class GameDemo : public Application {
public:
    GameDemo() : Application() {
        // Configure window
        WindowConfig config;
        config.width = 1280;
        config.height = 720;
        config.title = "Game Engine 2D - Advanced C++ Demo";
        config.fullscreen = false;
        config.vsync = true;
        
        // Set up callbacks
        setUpdateCallback([this](float deltaTime) {
            update(deltaTime);
        });
        
        setRenderCallback([this]() {
            render();
        });
        
        setEventCallback([this](const Event& event) {
            handleEvent(event);
        });
    }
    
    void update(float deltaTime) {
        // Game update logic
        static float rotation = 0.0f;
        rotation += deltaTime * 45.0f; // 45 degrees per second
        
        // Update game objects, physics, etc.
    }
    
    void render() {
        // Custom rendering logic
        // This is called after the default scene rendering
        
        // Example: Draw some debug information
        if (getWindow()) {
            auto renderer = getRenderer();
            if (renderer) {
                // Set a different clear color for demonstration
                renderer->setClearColor(Color(0.1f, 0.1f, 0.2f, 1.0f));
            }
        }
    }
    
    void handleEvent(const Event& event) {
        if (event.name == "key_press") {
            KeyCode key = *static_cast<const KeyCode*>(event.data);
            if (key == KeyCode::F1) {
                // Toggle fullscreen
                if (getWindow()) {
                    getWindow()->setFullscreen(!getWindow()->isFullscreen());
                }
            } else if (key == KeyCode::F2) {
                // Toggle VSync
                enableVSync(!getWindow()->isVSyncEnabled());
            } else if (key == KeyCode::F3) {
                // Print statistics
                printStatistics();
            }
        }
    }
    
    void printStatistics() {
        std::cout << "\n=== Game Engine Statistics ===" << std::endl;
        std::cout << "FPS: " << getFPS() << std::endl;
        std::cout << "Frame Time: " << (getFrameTime() * 1000.0f) << " ms" << std::endl;
        std::cout << "Delta Time: " << (getDeltaTime() * 1000.0f) << " ms" << std::endl;
        
        if (getWindow()) {
            std::cout << "Window Size: " << getWindow()->getWidth() << "x" << getWindow()->getHeight() << std::endl;
            std::cout << "VSync: " << (getWindow()->isVSyncEnabled() ? "Enabled" : "Disabled") << std::endl;
            std::cout << "Fullscreen: " << (getWindow()->isFullscreen() ? "Yes" : "No") << std::endl;
        }
        std::cout << "=============================" << std::endl;
    }
};

void printWelcomeMessage() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                    Game Engine 2D Demo                      ║" << std::endl;
    std::cout << "║              Advanced C++ Graphics Engine                   ║" << std::endl;
    std::cout << "╠══════════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  Features Demonstrated:                                     ║" << std::endl;
    std::cout << "║  • Modern C++17 with OpenGL 3.3+                           ║" << std::endl;
    std::cout << "║  • Advanced Graphics Pipeline                               ║" << std::endl;
    std::cout << "║  • Shader System with Hot Reloading                         ║" << std::endl;
    std::cout << "║  • Component-Based Architecture                             ║" << std::endl;
    std::cout << "║  • Physics Engine Integration                               ║" << std::endl;
    std::cout << "║  • Audio System with 3D Spatial Sound                       ║" << std::endl;
    std::cout << "║  • Particle Systems                                         ║" << std::endl;
    std::cout << "║  • Lighting System                                          ║" << std::endl;
    std::cout << "║  • Scene Management                                         ║" << std::endl;
    std::cout << "║  • Input Handling                                           ║" << std::endl;
    std::cout << "║  • Resource Management                                      ║" << std::endl;
    std::cout << "╠══════════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  Controls:                                                  ║" << std::endl;
    std::cout << "║  ESC - Exit                                                 ║" << std::endl;
    std::cout << "║  F1  - Toggle Fullscreen                                   ║" << std::endl;
    std::cout << "║  F2  - Toggle VSync                                        ║" << std::endl;
    std::cout << "║  F3  - Show Statistics                                     ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << "\n";
}

void printSystemInfo() {
    std::cout << "\n=== System Information ===" << std::endl;
    
    // OpenGL version
    const char* glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    const char* glVendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    const char* glRenderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    
    std::cout << "OpenGL Version: " << (glVersion ? glVersion : "Unknown") << std::endl;
    std::cout << "OpenGL Vendor: " << (glVendor ? glVendor : "Unknown") << std::endl;
    std::cout << "OpenGL Renderer: " << (glRenderer ? glRenderer : "Unknown") << std::endl;
    
    // GLFW version
    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    std::cout << "GLFW Version: " << major << "." << minor << "." << rev << std::endl;
    
    // GLEW version
    std::cout << "GLEW Version: " << glewGetString(GLEW_VERSION) << std::endl;
    
    std::cout << "=========================" << std::endl;
}

int main() {
    printWelcomeMessage();
    
    try {
        // Initialize logging
        Logger::getInstance().setLogLevel(LogLevel::INFO);
        Logger::getInstance().setLogFile("game_engine.log");
        Logger::getInstance().enableConsoleOutput(true);
        
        LOG_INFO("Starting Game Engine 2D Demo");
        
        // Create and initialize application
        GameDemo app;
        
        if (!app.initialize()) {
            std::cerr << "Failed to initialize application!" << std::endl;
            return 1;
        }
        
        // Print system information after OpenGL context is created
        printSystemInfo();
        
        // Run the application
        app.run();
        
        LOG_INFO("Game Engine 2D Demo completed successfully");
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        LOG_CRITICAL_FMT("Fatal error: {}", e.what());
        return 1;
    }
    
    std::cout << "\nThank you for trying the Game Engine 2D Demo!" << std::endl;
    std::cout << "This demonstrates advanced C++ programming techniques" << std::endl;
    std::cout << "including modern graphics programming, system architecture," << std::endl;
    std::cout << "and performance optimization." << std::endl;
    
    return 0;
}
