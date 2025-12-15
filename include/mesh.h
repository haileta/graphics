// Created by dengq on 8/29/25.
//

#ifndef MESH_H
#define MESH_H
#include <vector>
#include <string>
#include <glad/glad.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <cstddef>
#include <shaderprogram.h>

struct VertexAttribute {
    GLuint pos;         // attribute location (index) in the shader
    GLint size;         // number of components (e.g., 3 for vec3)
    GLenum type;        // GL_FLOAT, etc.
    GLboolean normalized;
    GLsizei stride;     // byte stride of the vertex
    size_t offset;      // byte offset to the first component
};

class Mesh {
public:
    Mesh(std::vector<float> v, std::vector<unsigned int> idx, GLuint id);
    Mesh(const std::string& path, GLuint id);
    ~Mesh();

    // Drawing
    void draw() const;
    // Bind/unbind VAO and all registered textures
    void bind() const;
    void unbind() const;

    // Resource cleanup (safe to call multiple times)
    void cleanup();
    bool loadOBJ_(const std::string& path);
private:
    // Buffer setup helpers
    void createBuffers_();

    GLuint VAO = 0, VBO = 0, EBO = 0;
    GLsizei indexCount = 0;

    GLuint shaderProgramID;
    std::vector<float> vertices = {};
    std::vector<unsigned int> indices = {};

};

#endif
