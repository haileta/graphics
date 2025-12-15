//
// Created by dengq on 11/18/25.
//

#ifndef DEMO_LIGHTS_CONFIG_H
#define DEMO_LIGHTS_CONFIG_H
#include <string>
#include <vector>
#include <glm.hpp>

#include "shaderprogram.h"


namespace gfx {

    struct DirLightConfig {
        glm::vec3 direction { -0.2f, -1.0f, -0.3f };
        glm::vec3 ambient   { 0.05f, 0.05f, 0.05f };
        glm::vec3 diffuse   { 0.6f,  0.6f,  0.6f  };
        glm::vec3 specular  { 0.8f,  0.8f,  0.8f  };
    };

    struct PointLightConfig {
        glm::vec3 position { 1.5f, 0.0f, 0.0f };

        float constant  = 1.0f;
        float linear    = 0.09f;
        float quadratic = 0.032f;

        glm::vec3 ambient  { 0.05f, 0.05f, 0.05f };
        glm::vec3 diffuse  { 0.8f,  0.8f,  0.8f  };
        glm::vec3 specular { 1.0f,  1.0f,  1.0f  };
    };

    struct SpotLightConfig {
        glm::vec3 position  { 0.0f, 0.0f, 0.0f };
        glm::vec3 direction { 0.0f, 0.0f, -1.0f };

        float cutOff      = glm::cos(glm::radians(12.5f));
        float outerCutOff = glm::cos(glm::radians(17.5f));

        float constant  = 1.0f;
        float linear    = 0.09f;
        float quadratic = 0.032f;

        glm::vec3 ambient  { 0.0f, 0.0f, 0.0f };
        glm::vec3 diffuse  { 1.0f, 1.0f, 1.0f };
        glm::vec3 specular { 1.0f, 1.0f, 1.0f };
    };

    struct SceneConfig {
        std::vector<DirLightConfig>   dirLights;
        std::vector<PointLightConfig> pointLights;
        std::vector<SpotLightConfig>  spotLights;
    };

// ------------------------------------------------------------------
// Default scene (1 dir, 1 point, 1 spot – but you can change sizes)
// ------------------------------------------------------------------
    inline SceneConfig makeDefaultSceneConfig()
    {
        SceneConfig cfg;

        cfg.dirLights.push_back(DirLightConfig{});
        cfg.pointLights.push_back(PointLightConfig{});
        cfg.spotLights.push_back(SpotLightConfig{});

        return cfg;
    }

// ------------------------------------------------------------------
// Lights: array-style helpers (variable count via vector::size())
// GLSL side: dirLights[i], pointLights[i], spotLights[i]
// ------------------------------------------------------------------

    inline void applyDirLights(ShaderProgram& shader,
                               const std::vector<DirLightConfig>& L,
                               const std::string& arrayName = "dirLights",
                               const std::string& countName = "numDirLights")
    {
        shader.use();
        int n = static_cast<int>(L.size());
        shader.setUniform(countName, n);      // can be 0
        for (int i = 0; i < n; ++i) {
            std::string base = arrayName + "[" + std::to_string(i) + "]";
            shader.setUniform(base + ".direction", L[i].direction);
            shader.setUniform(base + ".ambient",   L[i].ambient);
            shader.setUniform(base + ".diffuse",   L[i].diffuse);
            shader.setUniform(base + ".specular",  L[i].specular);
        }
    }

    inline void applyPointLights(ShaderProgram& shader,
                                 const std::vector<PointLightConfig>& L,
                                 const std::string& arrayName = "pointLights",
                                 const std::string& countName = "numPointLights")
    {
        shader.use();
        int n = static_cast<int>(L.size());
        shader.setUniform(countName, n);      // can be 0
        for (int i = 0; i < n; ++i) {
            std::string base = arrayName + "[" + std::to_string(i) + "]";
            shader.setUniform(base + ".position",  L[i].position);
            shader.setUniform(base + ".constant",  L[i].constant);
            shader.setUniform(base + ".linear",    L[i].linear);
            shader.setUniform(base + ".quadratic", L[i].quadratic);
            shader.setUniform(base + ".ambient",   L[i].ambient);
            shader.setUniform(base + ".diffuse",   L[i].diffuse);
            shader.setUniform(base + ".specular",  L[i].specular);
        }
    }

    inline void applySpotLights(ShaderProgram& shader,
                                const std::vector<SpotLightConfig>& L,
                                const std::string& arrayName = "spotLights",
                                const std::string& countName = "numSpotLights")
    {
        shader.use();
        int n = static_cast<int>(L.size());
        shader.setUniform(countName, n);      // can be 0
        for (int i = 0; i < n; ++i) {
            std::string base = arrayName + "[" + std::to_string(i) + "]";
            shader.setUniform(base + ".position",    L[i].position);
            shader.setUniform(base + ".direction",   L[i].direction);
            shader.setUniform(base + ".cutOff",      L[i].cutOff);
            shader.setUniform(base + ".outerCutOff", L[i].outerCutOff);
            shader.setUniform(base + ".constant",    L[i].constant);
            shader.setUniform(base + ".linear",      L[i].linear);
            shader.setUniform(base + ".quadratic",   L[i].quadratic);
            shader.setUniform(base + ".ambient",     L[i].ambient);
            shader.setUniform(base + ".diffuse",     L[i].diffuse);
            shader.setUniform(base + ".specular",    L[i].specular);
        }
    }

} // namespace gfx

#endif //DEMO_LIGHTS_CONFIG_H
