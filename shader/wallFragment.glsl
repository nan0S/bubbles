#version 330 core

struct PointLight
{
    vec3 pos;
};  

in vec3 fPosition;
in vec3 fNormal;

uniform vec3 cameraPos;
uniform int pointLightCount;
uniform PointLight pointLights[20];

out vec4 outColor;

vec3 wallColor = vec3(0, 0.3, 0.6);

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.pos - fPosition);

    vec3 ambient = vec3(0.1);

    float diff = clamp(dot(lightDir, normal), 0, 1);
    vec3 diffuse = vec3(0.3) * diff;

    vec3 r = reflect(-lightDir, normal);
    float spec = pow(max(dot(r, viewDir), 0), 10);
    vec3 specular = vec3(0.3) * spec;

    float dist = length(light.pos - fPosition);
    float mult = 1 / (0.1 * dist * dist + 0.1 * dist + 1.0);

    return (ambient + diffuse + specular) * wallColor * mult;
}

void main()
{
    PointLight light;
    light.pos = cameraPos;

    vec3 n = normalize(fNormal);
    vec3 e = normalize(cameraPos - fPosition);

    vec3 color = calcPointLight(light, n, e);
    for (int i = 0; i < pointLightCount; ++i)
        color += calcPointLight(pointLights[i], n, e);

    float dist = length(fPosition - cameraPos);
    float mult = exp(-dist / 6);
    color += mult * wallColor;

    outColor = vec4(color, 1);
}