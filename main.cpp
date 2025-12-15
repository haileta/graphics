#include <iostream>
#include <random>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shaderprogram.h"
#include "stb_image.h"
#include "mesh.h"
#include "camera.h"
#include "light_config.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int SCR_WIDTH = 720;
int SCR_HEIGHT = 720;
Camera camera(glm::vec3(0.0f, 2.0f, 8.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Better starting position

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lighting Scene - Hailemariam", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Skybox faces
    std::vector<std::string> faces{
        std::string(ASSETS_DIR) + "skybox/right.jpg",
        std::string(ASSETS_DIR) + "skybox/left.jpg",
        std::string(ASSETS_DIR) + "skybox/top.jpg",
        std::string(ASSETS_DIR) + "skybox/bottom.jpg",
        std::string(ASSETS_DIR) + "skybox/front.jpg",
        std::string(ASSETS_DIR) + "skybox/back.jpg"
    };

    // Container shader
    std::string vertPath = std::string(SHADER_DIR) + "cube_vertex.vert";
    std::string fragPath = std::string(SHADER_DIR) + "container_fragment.frag";
    ShaderProgram containerShaderProgram(vertPath, fragPath);
    GLuint cube_diffuse = containerShaderProgram.bindTexture2D("material.diffuse", std::string(ASSETS_DIR) + "container2.png", 0, false);
    GLuint cube_specular = containerShaderProgram.bindTexture2D("material.specular", std::string(ASSETS_DIR) + "container2_specular.png", 1, false);
    containerShaderProgram.bindCubeMap("skybox", faces, 2);
    containerShaderProgram.setUniform("material.shininess", 32.0f);
    containerShaderProgram.setUniform("material.alpha", 1.0f);

    // Light-cube shader (for visualizing point lights)
    vertPath = std::string(SHADER_DIR) + "cube_vertex.vert";
    fragPath = std::string(SHADER_DIR) + "light_fragment.frag";
    ShaderProgram lightShaderProgram(vertPath, fragPath);

    // Skybox shader
    vertPath = std::string(SHADER_DIR) + "skybox_vertex.vert";
    fragPath = std::string(SHADER_DIR) + "skybox_fragment.frag";
    ShaderProgram skyboxShaderProgram(vertPath, fragPath);
    skyboxShaderProgram.bindCubeMap("skybox", faces, 0);

    // Load meshes
    Mesh container(std::string(ASSETS_DIR) + "box.obj", containerShaderProgram.getID());
    Mesh lightMesh(std::string(ASSETS_DIR) + "box.obj", lightShaderProgram.getID());
    Mesh skybox(std::string(ASSETS_DIR) + "skybox.obj", skyboxShaderProgram.getID());

    // Cube positions
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 3.0f,  1.0f, -5.0f),
        glm::vec3(-3.0f,  0.5f, -4.0f),
        glm::vec3(-2.0f, -1.0f, -8.0f),
        glm::vec3( 2.5f, -0.5f, -3.0f),
        glm::vec3(-1.5f,  2.0f, -6.0f),
        glm::vec3( 1.5f, -1.5f, -2.0f),
        glm::vec3( 2.0f,  1.5f, -7.0f),
        glm::vec3( 0.5f,  0.5f, -1.5f),
        glm::vec3(-2.5f,  0.0f, -3.5f)
    };

    gfx::SceneConfig cfg = gfx::makeDefaultSceneConfig();

    // Directional light
    cfg.dirLights.clear();
    cfg.dirLights.resize(1);
    cfg.dirLights[0].direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    cfg.dirLights[0].ambient = glm::vec3(0.08f, 0.08f, 0.08f);
    cfg.dirLights[0].diffuse = glm::vec3(0.3f, 0.3f, 0.3f);
    cfg.dirLights[0].specular = glm::vec3(0.4f, 0.4f, 0.4f);

    cfg.pointLights.clear();
    cfg.pointLights.resize(3);

    // Purple
    cfg.pointLights[0].position = glm::vec3(4.0f, 3.0f, -2.0f);
    cfg.pointLights[0].diffuse  = glm::vec3(0.6f, 0.2f, 0.8f);
    cfg.pointLights[0].ambient  = cfg.pointLights[0].diffuse * 0.1f;
    cfg.pointLights[0].specular = cfg.pointLights[0].diffuse;
    cfg.pointLights[0].constant = 1.0f;
    cfg.pointLights[0].linear   = 0.07f;
    cfg.pointLights[0].quadratic= 0.017f;

   // pink
    cfg.pointLights[1].position = glm::vec3(-4.0f, 3.0f, -5.0f);
    cfg.pointLights[1].diffuse  = glm::vec3(1.0f, 0.4f, 0.7f);
    cfg.pointLights[1].ambient  = cfg.pointLights[1].diffuse * 0.1f;
    cfg.pointLights[1].specular = cfg.pointLights[1].diffuse;
    cfg.pointLights[1].constant = 1.0f;
    cfg.pointLights[1].linear   = 0.07f;
    cfg.pointLights[1].quadratic= 0.017f;

    // Orange
    cfg.pointLights[2].position = glm::vec3(0.0f, 4.0f, -8.0f);
    cfg.pointLights[2].diffuse  = glm::vec3(1.0f, 0.55f, 0.1f);  // Orange
    cfg.pointLights[2].ambient  = cfg.pointLights[2].diffuse * 0.1f;
    cfg.pointLights[2].specular = cfg.pointLights[2].diffuse;
    cfg.pointLights[2].constant = 1.0f;
    cfg.pointLights[2].linear   = 0.07f;
    cfg.pointLights[2].quadratic= 0.017f;

    // Spotlight (flashlight)
    cfg.spotLights.clear();
    cfg.spotLights.resize(1);
    cfg.spotLights[0].ambient  = glm::vec3(0.0f);
    cfg.spotLights[0].diffuse  = glm::vec3(2.0f, 2.0f, 2.0f);
    cfg.spotLights[0].specular = glm::vec3(2.5f, 2.5f, 2.5f);
    cfg.spotLights[0].constant = 1.0f;
    cfg.spotLights[0].linear   = 0.045f;
    cfg.spotLights[0].quadratic= 0.0075f;
    cfg.spotLights[0].cutOff   = glm::cos(glm::radians(12.5f));
    cfg.spotLights[0].outerCutOff = glm::cos(glm::radians(17.5f));

    // Upload static lights to shader (we will update the spotlight each frame)
    gfx::applyDirLights(containerShaderProgram, cfg.dirLights);
    gfx::applyPointLights(containerShaderProgram, cfg.pointLights);
    gfx::applySpotLights(containerShaderProgram, cfg.spotLights);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        camera.ProcessKeyboard(window, deltaTime);

        // Clear
        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update spotlight to follow camera
        cfg.spotLights[0].position = camera.Position;
        cfg.spotLights[0].direction = camera.Front;
        // Re-apply the spotlight uniforms every frame
        gfx::applySpotLights(containerShaderProgram, cfg.spotLights);

        // Matrices and view setup
        containerShaderProgram.use();
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjection((float)SCR_WIDTH / SCR_HEIGHT);
        containerShaderProgram.setUniform("view", view);
        containerShaderProgram.setUniform("projection", projection);
        containerShaderProgram.setUniform("viewPos", camera.Position);

        // Draw containers (with small rotation animation)
        for (int i = 0; i < 10; i++) {
            containerShaderProgram.setUniform("material.alpha", 1.0f);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i + currentFrame * 15.0f;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            containerShaderProgram.setUniform("model", model);
            container.draw();
        }

        // Skybox
        glDepthFunc(GL_LEQUAL);
        skyboxShaderProgram.use();
        glm::mat4 viewNoTrans = glm::mat4(glm::mat3(view));
        skyboxShaderProgram.setUniform("view", viewNoTrans);
        skyboxShaderProgram.setUniform("projection", projection);
        skybox.draw();
        glDepthFunc(GL_LESS);

        lightShaderProgram.use();
        lightShaderProgram.setUniform("view", view);
        lightShaderProgram.setUniform("projection", projection);

        float pulseScale = 0.3f + 0.1f * sin(currentFrame * 2.0f);

        for (int i = 0; i < (int)cfg.pointLights.size(); ++i) {
            glm::vec3 pos = cfg.pointLights[i].position;
            glm::vec3 lightColor = cfg.pointLights[i].diffuse;

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pos);
            model = glm::scale(model, glm::vec3(pulseScale));

            lightShaderProgram.setUniform("model", model);
            lightShaderProgram.setUniform("lightColor", lightColor);
            lightMesh.draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (camera.firstMouse) {
        camera.lastX = xpos;
        camera.lastY = ypos;
        camera.firstMouse = false;
    }

    float xoffset = xpos - camera.lastX;
    float yoffset = camera.lastY - ypos;

    camera.lastX = xpos;
    camera.lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
