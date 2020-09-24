#version 330 core

struct DirLight
{
    vec3 dir;

    // vec3 ambient;
    // vec3 specular;
    // vec3 diffuse;
};

struct PointLight
{
    vec3 pos;
    // float a;
    // float b;
    // float c;

    // vec3 ambient;
    // vec3 specular;
    // vec3 diffuse;
};  

in vec3 position;
in vec4 color;
in vec3 normal;

out vec4 outColor;

uniform DirLight dirLight;
uniform int pointLightCount;
uniform PointLight pointLights[20];
uniform vec3 cameraPos;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.dir);

    vec3 ambient = vec3(0.5, 0.5, 0.5);

    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = vec3(0.4, 0.4, 0.4) * diff;    

    vec3 r = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, r), 0), 10) * 0.5;
    vec3 specular = vec3(0.5, 0.5, 0.5) * spec;

    return (ambient + diffuse + specular) * color.xyz * 0.5;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.pos - position);
    if (length(light.pos - position) <= 0.5)
        return color.xyz * 0.2;

    vec3 ambient = vec3(0.05, 0.05, 0.05);

    float dist = length(light.pos - position);
    float mult = 1 / (0.1 * dist * dist + 0.09 * dist + 1.0);
    
    float diff = max(dot(normal, lightDir), 0) * 1;
    vec3 diffuse = vec3(0.8, 0.8, 0.8) * diff;
    
    vec3 r = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, r), 0), 5);
    vec3 specular = vec3(1, 1, 1) * spec;

    return (ambient + diffuse + specular) * color.xyz * mult;
}

void main()
{   
    vec3 n = normalize(normal);
    vec3 e = normalize(cameraPos - position);

    vec3 finalColor = calcDirLight(dirLight, n, e);
    for (int i = 0; i < pointLightCount; ++i)
        finalColor += calcPointLight(pointLights[i], n, e);
    
    PointLight additional;
    additional.pos = cameraPos;
    // additional.a = 0.05;
    // additional.b = 0.032;
    // additional.c = 1.0;
    // additional.ambient = vec3(0.05, 0.05, 0.05);
    // additional.diffuse = vec3(1, 1, 1);
    // additional.specular = vec3(0.3, 0.3, 0.3);

    finalColor += calcPointLight(additional, n, e);

    outColor = vec4(finalColor, color.a);
}