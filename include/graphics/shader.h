#pragma once

#include "types.h"
#include <string>
#include <unordered_map>
#include <memory>

namespace GameEngine2D {

class Shader {
public:
    Shader();
    ~Shader();
    
    // Shader lifecycle
    bool loadFromFile(const std::string& vertexPath, const std::string& fragmentPath);
    bool loadFromSource(const std::string& vertexSource, const std::string& fragmentSource);
    void destroy();
    
    // Shader usage
    void bind() const;
    void unbind() const;
    bool isValid() const { return m_programID != 0; }
    
    // Uniform setters
    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, const Vector2& value);
    void setUniform(const std::string& name, const Vector3& value);
    void setUniform(const std::string& name, const Vector4& value);
    void setUniform(const std::string& name, const Matrix3& value);
    void setUniform(const std::string& name, const Matrix4& value);
    void setUniform(const std::string& name, const Color& value);
    void setUniform(const std::string& name, bool value);
    
    // Array uniforms
    void setUniform(const std::string& name, const std::vector<int>& values);
    void setUniform(const std::string& name, const std::vector<float>& values);
    void setUniform(const std::string& name, const std::vector<Vector2>& values);
    void setUniform(const std::string& name, const std::vector<Vector3>& values);
    void setUniform(const std::string& name, const std::vector<Vector4>& values);
    
    // Texture uniforms
    void setUniform(const std::string& name, int textureUnit);
    
    // Shader introspection
    int getUniformLocation(const std::string& name) const;
    int getAttributeLocation(const std::string& name) const;
    std::vector<std::string> getUniformNames() const;
    std::vector<std::string> getAttributeNames() const;
    
    // Shader program ID
    unsigned int getProgramID() const { return m_programID; }

private:
    unsigned int m_programID;
    mutable std::unordered_map<std::string, int> m_uniformCache;
    mutable std::unordered_map<std::string, int> m_attributeCache;
    
    // Shader compilation
    unsigned int compileShader(const std::string& source, unsigned int type);
    bool linkProgram(unsigned int vertexShader, unsigned int fragmentShader);
    std::string getShaderInfoLog(unsigned int shader);
    std::string getProgramInfoLog(unsigned int program);
    
    // Cache management
    int getCachedUniformLocation(const std::string& name) const;
    int getCachedAttributeLocation(const std::string& name) const;
    
    // Shader introspection
    void introspectProgram();
    std::vector<std::string> getActiveUniforms() const;
    std::vector<std::string> getActiveAttributes() const;
};

// Shader manager for caching and managing shaders
class ShaderManager {
public:
    static ShaderManager& getInstance();
    
    // Shader management
    std::shared_ptr<Shader> loadShader(const std::string& name, 
                                      const std::string& vertexPath, 
                                      const std::string& fragmentPath);
    std::shared_ptr<Shader> getShader(const std::string& name);
    bool unloadShader(const std::string& name);
    void unloadAll();
    
    // Shader creation helpers
    std::shared_ptr<Shader> createBasicShader();
    std::shared_ptr<Shader> createTextureShader();
    std::shared_ptr<Shader> createColorShader();
    std::shared_ptr<Shader> createParticleShader();
    std::shared_ptr<Shader> createLightingShader();
    
    // Statistics
    size_t getShaderCount() const { return m_shaders.size(); }
    std::vector<std::string> getShaderNames() const;

private:
    ShaderManager() = default;
    ~ShaderManager() = default;
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;
    
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
};

} // namespace GameEngine2D
