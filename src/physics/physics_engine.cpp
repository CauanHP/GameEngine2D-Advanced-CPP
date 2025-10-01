#include "physics/physics_engine.h"
#include "utils/logger.h"

namespace GameEngine2D {

PhysicsEngine::PhysicsEngine() {
}

PhysicsEngine::~PhysicsEngine() {
    shutdown();
}

bool PhysicsEngine::initialize() {
    LOG_INFO("PhysicsEngine initialized");
    return true;
}

void PhysicsEngine::shutdown() {
    LOG_INFO("PhysicsEngine shutdown");
}

void PhysicsEngine::update(float deltaTime) {
    // Physics update logic
}

} // namespace GameEngine2D
