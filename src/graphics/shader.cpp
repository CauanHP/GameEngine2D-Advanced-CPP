#include "graphics/shader.h"
#include "utils/logger.h"
#include "utils/file_utils.h"
#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <iostream>

namespace GameEngine2D {

Shader::Shader() : m_programID(0) {
}

Shader::~Shader() {
    destroy();
}

bool Shader::loadFromFile(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexSource = FileUtils::readTextFile(vertexPath);
    std::string fragmentSource = FileUtils::readTextFile(fragmentPath);
    
    if (vertexSource.empty() || fragmentSource.empty()) {
        LOG_ERROR_FMT("Failed to load shader files: {} or {}", vertexPath, fragmentPath);
        return false;
    }
    
    return loadFromSource(vertexSource, fragmentSource);
}

bool Shader::loadFromSource(const std::string& vertexSource, const std::string& fragmentSource) {
    // Compile vertex shader
    unsigned int vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    if (vertexShader == 0) {
        LOG_ERROR("Failed to compile vertex shader");
        return false;
    }
    
    // Compile fragment shader
    unsigned int fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);
    if (fragmentShader == 0) {
        LOG_ERROR("Failed to compile fragment shader");
        glDeleteShader(vertexShader);
        return false;
    }
    
    // Link program
    if (!linkProgram(vertexShader, fragmentShader)) {
        LOG_ERROR("Failed to link shader program");
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }
    
    // Clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Introspect the program
    introspectProgram();
    
    LOG_INFO("Shader program created successfully");
    return true;
}

void Shader::destroy() {
    if (m_programID != 0) {
        glDeleteProgram(m_programID);
        m_programID = 0;
        m_uniformCache.clear();
        m_attributeCache.clear();
    }
}

void Shader::bind() const {
    if (m_programID != 0) {
        glUseProgram(m_programID);
    }
}

void Shader::unbind() const {
    glUseProgram(0);
}

void Shader::setUniform(const std::string& name, int value) {
    int location = getCachedUniformLocation(name);
    if (location != -1) {
        glUniform1i(location, value);
    }
}

void Shader::setUniform(const std::string& name, float value) {
    int location = getCachedUniformLocation(name);
    if (location != -1) {
        glUniform1f(location, value);
    }
}

void Shader::setUniform(const std::string& name, const Vector2& value) {
    int location = getCachedUniformLocation(name);
    if (location != -1) {
        glUniform2f(location, value.x, value.y);
    }
}

void Shader::setUniform(const std::string& name, const Vector3& value) {
    int location = getCachedUniformLocation(name);
    if (location != -1) {
        glUniform3f(location, value.x, value.y, value.z);
    }
}

void Shader::setUniform(const std::string& name, const Vector4& value) {
    int location = getCachedUniformLocation(name);
    if (location != -1) {
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }
}

void Shader::setUniform(const std::string& name, const Matrix3& value) {
    int location = getCachedUniformLocation(name);
    if (location != -1) {
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
}

void Shader::setUniform(const std::string& name, const Matrix4& value) {
    int location = getCachedUniformLocation(name);
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
}

void Shader::setUniform(const std::string& name, const Color& value) {
    setUniform(name, Vector4(value.r, value.g, value.b, value.a));
}

void Shader::setUniform(const std::string& name, bool value) {
    setUniform(name, value ? 1 : 0);
}

void Shader::setUniform(const std::string& name, const std::vector<int>& values) {
    int location = getCachedUniformLocation(name);
    if (location != -1 && !values.empty()) {
        glUniform1iv(location, static_cast<GLsizei>(values.size()), values.data());
    }
}

void Shader::setUniform(const std::string& name, const std::vector<float>& values) {
    int location = getCachedUniformLocation(name);
    if (location != -1 && !values.empty()) {
        glUniform1fv(location, static_cast<GLsizei>(values.size()), values.data());
    }
}

void Shader::setUniform(const std::string& name, const std::vector<Vector2>& values) {
    int location = getCachedUniformLocation(name);
    if (location != -1 && !values.empty()) {
        glUniform2fv(location, static_cast<GLsizei>(values.size()), reinterpret_cast<const float*>(values.data()));
    }
}

void Shader::setUniform(const std::string& name, const std::vector<Vector3>& values) {
    int location = getCachedUniformLocation(name);
    if (location != -1 && !values.empty()) {
        glUniform3fv(location, static_cast<GLsizei>(values.size()), reinterpret_cast<const float*>(values.data()));
    }
}

void Shader::setUniform(const std::string& name, const std::vector<Vector4>& values) {
    int location = getCachedUniformLocation(name);
    if (location != -1 && !values.empty()) {
        glUniform4fv(location, static_cast<GLsizei>(values.size()), reinterpret_cast<const float*>(values.data()));
    }
}

void Shader::setUniform(const std::string& name, int textureUnit) {
    setUniform(name, textureUnit);
}

int Shader::getUniformLocation(const std::string& name) const {
    return getCachedUniformLocation(name);
}

int Shader::getAttributeLocation(const std::string& name) const {
    return getCachedAttributeLocation(name);
}

std::vector<std::string> Shader::getUniformNames() const {
    std::vector<std::string> names;
    for (const auto& pair : m_uniformCache) {
        names.push_back(pair.first);
    }
    return names;
}

std::vector<std::string> Shader::getAttributeNames() const {
    std::vector<std::string> names;
    for (const auto& pair : m_attributeCache) {
        names.push_back(pair.first);
    }
    return names;
}

unsigned int Shader::compileShader(const std::string& source, unsigned int type) {
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    
    // Check compilation status
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::string infoLog = getShaderInfoLog(shader);
        LOG_ERROR_FMT("Shader compilation failed: {}", infoLog);
        glDeleteShader(shader);
        return 0;
    }
    
    return shader;
}

bool Shader::linkProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    m_programID = glCreateProgram();
    glAttachShader(m_programID, vertexShader);
    glAttachShader(m_programID, fragmentShader);
    glLinkProgram(m_programID);
    
    // Check linking status
    int success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success) {
        std::string infoLog = getProgramInfoLog(m_programID);
        LOG_ERROR_FMT("Shader program linking failed: {}", infoLog);
        glDeleteProgram(m_programID);
        m_programID = 0;
        return false;
    }
    
    return true;
}

std::string Shader::getShaderInfoLog(unsigned int shader) {
    int length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    
    if (length > 0) {
        std::string infoLog(length, '\0');
        glGetShaderInfoLog(shader, length, &length, &infoLog[0]);
        return infoLog;
    }
    
    return "";
}

std::string Shader::getProgramInfoLog(unsigned int program) {
    int length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    
    if (length > 0) {
        std::string infoLog(length, '\0');
        glGetProgramInfoLog(program, length, &length, &infoLog[0]);
        return infoLog;
    }
    
    return "";
}

int Shader::getCachedUniformLocation(const std::string& name) const {
    auto it = m_uniformCache.find(name);
    if (it != m_uniformCache.end()) {
        return it->second;
    }
    
    int location = glGetUniformLocation(m_programID, name.c_str());
    m_uniformCache[name] = location;
    
    if (location == -1) {
        LOG_WARNING_FMT("Uniform '{}' not found in shader", name);
    }
    
    return location;
}

int Shader::getCachedAttributeLocation(const std::string& name) const {
    auto it = m_attributeCache.find(name);
    if (it != m_attributeCache.end()) {
        return it->second;
    }
    
    int location = glGetAttribLocation(m_programID, name.c_str());
    m_attributeCache[name] = location;
    
    if (location == -1) {
        LOG_WARNING_FMT("Attribute '{}' not found in shader", name);
    }
    
    return location;
}

void Shader::introspectProgram() {
    // Get active uniforms
    int uniformCount;
    glGetProgramiv(m_programID, GL_ACTIVE_UNIFORMS, &uniformCount);
    
    for (int i = 0; i < uniformCount; ++i) {
        char name[256];
        int length, size;
        unsigned int type;
        glGetActiveUniform(m_programID, i, sizeof(name), &length, &size, &type, name);
        
        int location = glGetUniformLocation(m_programID, name);
        m_uniformCache[name] = location;
    }
    
    // Get active attributes
    int attributeCount;
    glGetProgramiv(m_programID, GL_ACTIVE_ATTRIBUTES, &attributeCount);
    
    for (int i = 0; i < attributeCount; ++i) {
        char name[256];
        int length, size;
        unsigned int type;
        glGetActiveAttrib(m_programID, i, sizeof(name), &length, &size, &type, name);
        
        int location = glGetAttribLocation(m_programID, name);
        m_attributeCache[name] = location;
    }
}

// ShaderManager implementation
ShaderManager& ShaderManager::getInstance() {
    static ShaderManager instance;
    return instance;
}

std::shared_ptr<Shader> ShaderManager::loadShader(const std::string& name, 
                                                  const std::string& vertexPath, 
                                                  const std::string& fragmentPath) {
    auto it = m_shaders.find(name);
    if (it != m_shaders.end()) {
        return it->second;
    }
    
    auto shader = std::make_shared<Shader>();
    if (shader->loadFromFile(vertexPath, fragmentPath)) {
        m_shaders[name] = shader;
        LOG_INFO_FMT("Loaded shader: {}", name);
        return shader;
    }
    
    LOG_ERROR_FMT("Failed to load shader: {}", name);
    return nullptr;
}

std::shared_ptr<Shader> ShaderManager::getShader(const std::string& name) {
    auto it = m_shaders.find(name);
    return (it != m_shaders.end()) ? it->second : nullptr;
}

bool ShaderManager::unloadShader(const std::string& name) {
    auto it = m_shaders.find(name);
    if (it != m_shaders.end()) {
        m_shaders.erase(it);
        LOG_INFO_FMT("Unloaded shader: {}", name);
        return true;
    }
    return false;
}

void ShaderManager::unloadAll() {
    m_shaders.clear();
    LOG_INFO("All shaders unloaded");
}

std::shared_ptr<Shader> ShaderManager::createBasicShader() {
    const std::string vertexSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPosition;
        layout (location = 1) in vec2 aTexCoord;
        layout (location = 2) in vec4 aColor;
        
        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProjection;
        
        out vec2 TexCoord;
        out vec4 Color;
        
        void main() {
            gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
            TexCoord = aTexCoord;
            Color = aColor;
        }
    )";
    
    const std::string fragmentSource = R"(
        #version 330 core
        in vec2 TexCoord;
        in vec4 Color;
        
        uniform sampler2D uTexture;
        uniform bool uUseTexture;
        
        out vec4 FragColor;
        
        void main() {
            if (uUseTexture) {
                FragColor = texture(uTexture, TexCoord) * Color;
            } else {
                FragColor = Color;
            }
        }
    )";
    
    auto shader = std::make_shared<Shader>();
    if (shader->loadFromSource(vertexSource, fragmentSource)) {
        m_shaders["basic"] = shader;
        return shader;
    }
    
    return nullptr;
}

std::shared_ptr<Shader> ShaderManager::createTextureShader() {
    return createBasicShader(); // Same as basic for now
}

std::shared_ptr<Shader> ShaderManager::createColorShader() {
    return createBasicShader(); // Same as basic for now
}

std::shared_ptr<Shader> ShaderManager::createParticleShader() {
    const std::string vertexSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPosition;
        layout (location = 1) in vec2 aTexCoord;
        layout (location = 2) in vec4 aColor;
        layout (location = 3) in float aSize;
        
        uniform mat4 uView;
        uniform mat4 uProjection;
        
        out vec2 TexCoord;
        out vec4 Color;
        out float Size;
        
        void main() {
            gl_Position = uProjection * uView * vec4(aPosition, 1.0);
            gl_PointSize = aSize;
            TexCoord = aTexCoord;
            Color = aColor;
            Size = aSize;
        }
    )";
    
    const std::string fragmentSource = R"(
        #version 330 core
        in vec2 TexCoord;
        in vec4 Color;
        in float Size;
        
        uniform sampler2D uTexture;
        
        out vec4 FragColor;
        
        void main() {
            vec2 center = gl_PointCoord - vec2(0.5);
            float dist = length(center);
            if (dist > 0.5) discard;
            
            FragColor = texture(uTexture, gl_PointCoord) * Color;
        }
    )";
    
    auto shader = std::make_shared<Shader>();
    if (shader->loadFromSource(vertexSource, fragmentSource)) {
        m_shaders["particle"] = shader;
        return shader;
    }
    
    return nullptr;
}

std::shared_ptr<Shader> ShaderManager::createLightingShader() {
    const std::string vertexSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPosition;
        layout (location = 1) in vec2 aTexCoord;
        layout (location = 2) in vec3 aNormal;
        
        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProjection;
        uniform mat3 uNormalMatrix;
        
        out vec2 TexCoord;
        out vec3 Normal;
        out vec3 FragPos;
        
        void main() {
            gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
            TexCoord = aTexCoord;
            Normal = uNormalMatrix * aNormal;
            FragPos = vec3(uModel * vec4(aPosition, 1.0));
        }
    )";
    
    const std::string fragmentSource = R"(
        #version 330 core
        in vec2 TexCoord;
        in vec3 Normal;
        in vec3 FragPos;
        
        uniform sampler2D uTexture;
        uniform vec3 uLightPos;
        uniform vec3 uLightColor;
        uniform vec3 uViewPos;
        
        out vec4 FragColor;
        
        void main() {
            vec3 color = texture(uTexture, TexCoord).rgb;
            
            // Ambient
            float ambientStrength = 0.1;
            vec3 ambient = ambientStrength * uLightColor;
            
            // Diffuse
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(uLightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * uLightColor;
            
            vec3 result = (ambient + diffuse) * color;
            FragColor = vec4(result, 1.0);
        }
    )";
    
    auto shader = std::make_shared<Shader>();
    if (shader->loadFromSource(vertexSource, fragmentSource)) {
        m_shaders["lighting"] = shader;
        return shader;
    }
    
    return nullptr;
}

std::vector<std::string> ShaderManager::getShaderNames() const {
    std::vector<std::string> names;
    for (const auto& pair : m_shaders) {
        names.push_back(pair.first);
    }
    return names;
}

} // namespace GameEngine2D
