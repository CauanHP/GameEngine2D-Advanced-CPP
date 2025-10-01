cmake_minimum_required(VERSION 3.16)
project(GameEngine2D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

# Include directories
include_directories(include)
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})

# Source files
set(SOURCES
    src/main.cpp
    src/core/application.cpp
    src/core/window.cpp
    src/core/input_manager.cpp
    src/core/time_manager.cpp
    src/graphics/renderer.cpp
    src/graphics/shader.cpp
    src/graphics/texture.cpp
    src/graphics/sprite.cpp
    src/graphics/camera.cpp
    src/graphics/batch_renderer.cpp
    src/physics/physics_engine.cpp
    src/physics/rigidbody.cpp
    src/physics/collision_detector.cpp
    src/physics/spatial_hash.cpp
    src/audio/audio_manager.cpp
    src/audio/sound.cpp
    src/scene/scene_manager.cpp
    src/scene/game_object.cpp
    src/scene/component.cpp
    src/scene/transform.cpp
    src/scene/sprite_renderer.cpp
    src/scene/animator.cpp
    src/utils/math_utils.cpp
    src/utils/file_utils.cpp
    src/utils/logger.cpp
    src/systems/particle_system.cpp
    src/systems/lighting_system.cpp
    src/systems/input_system.cpp
    src/systems/render_system.cpp
    src/systems/physics_system.cpp
)

# Header files
set(HEADERS
    include/core/application.h
    include/core/window.h
    include/core/input_manager.h
    include/core/time_manager.h
    include/graphics/renderer.h
    include/graphics/shader.h
    include/graphics/texture.h
    include/graphics/sprite.h
    include/graphics/camera.h
    include/graphics/batch_renderer.h
    include/physics/physics_engine.h
    include/physics/rigidbody.h
    include/physics/collision_detector.h
    include/physics/spatial_hash.h
    include/audio/audio_manager.h
    include/audio/sound.h
    include/scene/scene_manager.h
    include/scene/game_object.h
    include/scene/component.h
    include/scene/transform.h
    include/scene/sprite_renderer.h
    include/scene/animator.h
    include/utils/math_utils.h
    include/utils/file_utils.h
    include/utils/logger.h
    include/systems/particle_system.h
    include/systems/lighting_system.h
    include/systems/input_system.h
    include/systems/render_system.h
    include/systems/physics_system.h
    include/types.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    OpenGL::GL
    glfw
    GLEW::GLEW
    glm::glm
)

# Compiler flags
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -O2)

# Copy shaders to build directory
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Enable testing
enable_testing()
add_subdirectory(tests)
