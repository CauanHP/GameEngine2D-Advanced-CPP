#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include <chrono>

namespace GameEngine2D {

// Forward declarations
class GameObject;
class Component;
class Scene;
class Renderer;
class PhysicsEngine;

// Basic types
using Vector2 = glm::vec2;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;
using Matrix3 = glm::mat3;
using Matrix4 = glm::mat4;
using Color = glm::vec4;
using TimePoint = std::chrono::high_resolution_clock::time_point;
using Duration = std::chrono::duration<float>;

// Entity and component IDs
using EntityID = uint32_t;
using ComponentID = uint32_t;

// Resource IDs
using TextureID = uint32_t;
using ShaderID = uint32_t;
using SoundID = uint32_t;

// Window and input types
struct WindowConfig {
    int width = 1280;
    int height = 720;
    std::string title = "Game Engine 2D";
    bool fullscreen = false;
    bool vsync = true;
    bool resizable = true;
};

enum class KeyCode {
    UNKNOWN = -1,
    SPACE = 32,
    APOSTROPHE = 39,
    COMMA = 44,
    MINUS = 45,
    PERIOD = 46,
    SLASH = 47,
    NUM_0 = 48, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,
    SEMICOLON = 59,
    EQUAL = 61,
    A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    LEFT_BRACKET = 91,
    BACKSLASH = 92,
    RIGHT_BRACKET = 93,
    GRAVE_ACCENT = 96,
    ESCAPE = 256,
    ENTER = 257,
    TAB = 258,
    BACKSPACE = 259,
    INSERT = 260,
    DELETE = 261,
    RIGHT = 262,
    LEFT = 263,
    DOWN = 264,
    UP = 265,
    PAGE_UP = 266,
    PAGE_DOWN = 267,
    HOME = 268,
    END = 269,
    CAPS_LOCK = 280,
    SCROLL_LOCK = 281,
    NUM_LOCK = 282,
    PRINT_SCREEN = 283,
    PAUSE = 284,
    F1 = 290, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    LEFT_SHIFT = 340,
    LEFT_CONTROL = 341,
    LEFT_ALT = 342,
    LEFT_SUPER = 343,
    RIGHT_SHIFT = 344,
    RIGHT_CONTROL = 345,
    RIGHT_ALT = 346,
    RIGHT_SUPER = 347,
    MENU = 348
};

enum class MouseButton {
    BUTTON_1 = 0,
    BUTTON_2 = 1,
    BUTTON_3 = 2,
    BUTTON_4 = 3,
    BUTTON_5 = 4,
    BUTTON_6 = 5,
    BUTTON_7 = 6,
    BUTTON_8 = 7,
    LEFT = BUTTON_1,
    RIGHT = BUTTON_2,
    MIDDLE = BUTTON_3
};

enum class InputAction {
    RELEASE = 0,
    PRESS = 1,
    REPEAT = 2
};

// Graphics types
enum class PrimitiveType {
    POINTS = 0,
    LINES = 1,
    LINE_LOOP = 2,
    LINE_STRIP = 3,
    TRIANGLES = 4,
    TRIANGLE_STRIP = 5,
    TRIANGLE_FAN = 6
};

enum class BlendMode {
    NONE = 0,
    ALPHA = 1,
    ADDITIVE = 2,
    MULTIPLY = 3,
    SCREEN = 4
};

enum class FilterMode {
    NEAREST = 0,
    LINEAR = 1,
    NEAREST_MIPMAP_NEAREST = 2,
    LINEAR_MIPMAP_NEAREST = 3,
    NEAREST_MIPMAP_LINEAR = 4,
    LINEAR_MIPMAP_LINEAR = 5
};

enum class WrapMode {
    REPEAT = 0,
    MIRRORED_REPEAT = 1,
    CLAMP_TO_EDGE = 2,
    CLAMP_TO_BORDER = 3
};

struct TextureConfig {
    FilterMode minFilter = FilterMode::LINEAR;
    FilterMode magFilter = FilterMode::LINEAR;
    WrapMode wrapS = WrapMode::REPEAT;
    WrapMode wrapT = WrapMode::REPEAT;
    bool generateMipmaps = true;
    bool flipVertically = true;
};

// Physics types
enum class BodyType {
    STATIC = 0,
    KINEMATIC = 1,
    DYNAMIC = 2
};

enum class CollisionShape {
    NONE = 0,
    CIRCLE = 1,
    BOX = 2,
    POLYGON = 3,
    EDGE = 4
};

struct PhysicsMaterial {
    float density = 1.0f;
    float friction = 0.5f;
    float restitution = 0.3f;
    bool isSensor = false;
};

struct CollisionInfo {
    EntityID entityA;
    EntityID entityB;
    Vector2 contactPoint;
    Vector2 normal;
    float penetration;
    bool isTrigger;
};

// Animation types
enum class AnimationType {
    LINEAR = 0,
    EASE_IN = 1,
    EASE_OUT = 2,
    EASE_IN_OUT = 3,
    BOUNCE = 4,
    ELASTIC = 5
};

struct AnimationKeyframe {
    float time;
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    Color color;
};

struct AnimationClip {
    std::string name;
    float duration;
    std::vector<AnimationKeyframe> keyframes;
    bool loop = true;
};

// Particle system types
enum class ParticleType {
    POINT = 0,
    SPRITE = 1,
    MESH = 2
};

struct ParticleConfig {
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    Color startColor;
    Color endColor;
    float startSize;
    float endSize;
    float lifetime;
    float rotation;
    float angularVelocity;
    ParticleType type = ParticleType::POINT;
};

struct ParticleSystemConfig {
    int maxParticles = 1000;
    float emissionRate = 10.0f;
    float particleLifetime = 2.0f;
    Vector3 gravity = Vector3(0, -9.81f, 0);
    bool worldSpace = true;
    BlendMode blendMode = BlendMode::ALPHA;
};

// Lighting types
enum class LightType {
    DIRECTIONAL = 0,
    POINT = 1,
    SPOT = 2
};

struct Light {
    LightType type = LightType::POINT;
    Vector3 position;
    Vector3 direction;
    Color color = Color(1.0f, 1.0f, 1.0f, 1.0f);
    float intensity = 1.0f;
    float range = 10.0f;
    float innerConeAngle = 30.0f;
    float outerConeAngle = 45.0f;
    bool enabled = true;
};

// Audio types
enum class AudioFormat {
    MONO8 = 0,
    MONO16 = 1,
    STEREO8 = 2,
    STEREO16 = 3
};

struct AudioConfig {
    float volume = 1.0f;
    float pitch = 1.0f;
    bool loop = false;
    bool spatial = false;
    Vector3 position;
    float minDistance = 1.0f;
    float maxDistance = 100.0f;
};

// Event system types
using EventID = uint32_t;
using EventCallback = std::function<void(const void*)>;

struct Event {
    EventID id;
    std::string name;
    void* data = nullptr;
    size_t dataSize = 0;
    bool handled = false;
};

// Scene management types
enum class SceneState {
    LOADING = 0,
    ACTIVE = 1,
    PAUSED = 2,
    UNLOADING = 3
};

// Utility types
struct Rectangle {
    float x, y, width, height;
    
    Rectangle() : x(0), y(0), width(0), height(0) {}
    Rectangle(float x, float y, float w, float h) : x(x), y(y), width(w), height(h) {}
    
    bool contains(const Vector2& point) const {
        return point.x >= x && point.x <= x + width &&
               point.y >= y && point.y <= y + height;
    }
    
    bool intersects(const Rectangle& other) const {
        return !(x + width < other.x || other.x + other.width < x ||
                 y + height < other.y || other.y + other.height < y);
    }
};

struct Circle {
    Vector2 center;
    float radius;
    
    Circle() : center(0, 0), radius(0) {}
    Circle(const Vector2& c, float r) : center(c), radius(r) {}
    
    bool contains(const Vector2& point) const {
        float distance = glm::length(point - center);
        return distance <= radius;
    }
    
    bool intersects(const Circle& other) const {
        float distance = glm::length(center - other.center);
        return distance <= (radius + other.radius);
    }
};

// Constants
constexpr float PI = 3.14159265359f;
constexpr float DEG_TO_RAD = PI / 180.0f;
constexpr float RAD_TO_DEG = 180.0f / PI;

// Color constants
constexpr Color COLOR_WHITE = Color(1.0f, 1.0f, 1.0f, 1.0f);
constexpr Color COLOR_BLACK = Color(0.0f, 0.0f, 0.0f, 1.0f);
constexpr Color COLOR_RED = Color(1.0f, 0.0f, 0.0f, 1.0f);
constexpr Color COLOR_GREEN = Color(0.0f, 1.0f, 0.0f, 1.0f);
constexpr Color COLOR_BLUE = Color(0.0f, 0.0f, 1.0f, 1.0f);
constexpr Color COLOR_YELLOW = Color(1.0f, 1.0f, 0.0f, 1.0f);
constexpr Color COLOR_CYAN = Color(0.0f, 1.0f, 1.0f, 1.0f);
constexpr Color COLOR_MAGENTA = Color(1.0f, 0.0f, 1.0f, 1.0f);
constexpr Color COLOR_TRANSPARENT = Color(0.0f, 0.0f, 0.0f, 0.0f);

} // namespace GameEngine2D
