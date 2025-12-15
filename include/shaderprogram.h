#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "stb_image.h"

class ShaderProgram {
public:
    ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
    ~ShaderProgram();

    void use() const;
    GLuint getID() const;

    void destroy();

    // Set uniform variables of various types
    void setUniform(const std::string& name, int value) const;
    void setUniform(const std::string& name, float value) const;
    void setUniform(const std::string& name, const glm::vec3& value) const;
    void setUniform(const std::string& name, const glm::mat4& value) const;
    GLuint bindTexture2D(const std::string& samplerName,
                         const std::string& filePath,
                         GLint textureUnit,
                         bool flipVertical = true,
                         GLint wrap = GL_REPEAT,
                         GLint minFilter = GL_LINEAR_MIPMAP_LINEAR,
                         GLint magFilter = GL_LINEAR,
                         bool generateMipmaps = true) const;


    GLuint bindCubeMap(const std::string& samplerName,
                       const std::vector<std::string>& faces,
                       GLint textureUnit,
                       bool flipVertical = false,
                       GLint wrap = GL_CLAMP_TO_EDGE,
                       GLint minFilter = GL_LINEAR,
                       GLint magFilter = GL_LINEAR) const;




private:
    GLuint ID;
    bool isDeleted = false;

    std::string loadShaderSource(const std::string& filePath);
    static GLuint compileShader(const std::string& source, GLenum shaderType);
    void linkProgram(GLuint vertexShader, GLuint fragmentShader);
};

#endif