#version 410 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

// ---------------------------------------------------------------------
// Structs
// ---------------------------------------------------------------------
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    float alpha;
};

uniform Material material;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// ---------------------------------------------------------------------
// MAX LIGHTS
// ---------------------------------------------------------------------
#define MAX_DIR_LIGHTS   4
#define MAX_POINT_LIGHTS 8
#define MAX_SPOT_LIGHTS  4

uniform int numDirLights;
uniform int numPointLights;
uniform int numSpotLights;
uniform samplerCube skybox;

uniform DirLight   dirLights[MAX_DIR_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight  spotLights[MAX_SPOT_LIGHTS];

uniform vec3 viewPos;

// ---------------------------------------------------------------------
// Function declarations
// ---------------------------------------------------------------------
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

// ---------------------------------------------------------------------
// MAIN
// ---------------------------------------------------------------------
void main()
{
    vec3 norm    = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);

    // Directional
    for (int i = 0; i < numDirLights; ++i)
        result += CalcDirLight(dirLights[i], norm, viewDir);

    // Point lights
    for (int i = 0; i < numPointLights; ++i)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    // Spot lights
    for (int i = 0; i < numSpotLights; ++i)
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);

    // ---------------------------------------------------------------------
    // FIXED: Reduce skybox reflection (no longer overrides your light colors)
    // ---------------------------------------------------------------------
    float maskValue = texture(material.specular, TexCoords).r;
    float reflectionStrength = 0.2;    // <-- adjust if you want stronger reflection
    vec3 R = reflect(-viewDir, norm);
    vec3 envColor = texture(skybox, R).rgb;

    vec3 finalColor = mix(result, envColor, maskValue * reflectionStrength);

    FragColor = vec4(finalColor, material.alpha);
}

// ---------------------------------------------------------------------
// LIGHT CALCULATIONS
// ---------------------------------------------------------------------
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 texDiffuse = texture(material.diffuse, TexCoords).rgb;
    vec3 texSpec    = texture(material.specular, TexCoords).rgb;

    vec3 ambient  = light.ambient  * texDiffuse;
    vec3 diffuse  = light.diffuse  * diff * texDiffuse;
    vec3 specular = light.specular * spec * texSpec;

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 /
        (light.constant +
         light.linear * distance +
         light.quadratic * distance * distance);

    vec3 texDiffuse = texture(material.diffuse, TexCoords).rgb;
    vec3 texSpec    = texture(material.specular, TexCoords).rgb;

    vec3 ambient  = light.ambient  * texDiffuse;
    vec3 diffuse  = light.diffuse  * diff * texDiffuse;
    vec3 specular = light.specular * spec * texSpec;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 /
        (light.constant +
         light.linear * distance +
         light.quadratic * distance * distance);

    float theta   = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 texDiffuse = texture(material.diffuse, TexCoords).rgb;
    vec3 texSpec    = texture(material.specular, TexCoords).rgb;

    vec3 ambient  = light.ambient  * texDiffuse;
    vec3 diffuse  = light.diffuse  * diff * texDiffuse;
    vec3 specular = light.specular * spec * texSpec;

    ambient  *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}
