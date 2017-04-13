#version 450 core

// Uniforms
uniform vec3 cameraPosition;
uniform mat4 model;

// Light related
#define MAX_LIGHTS 10
uniform int numLights;

uniform struct Light {
  vec4 position;
  vec3 color;
  float attenuation;
  float ambientStrength;
} lights[MAX_LIGHTS];

// Fragment properties
in vec3 fragmentColor;
in vec3 fragmentNormal;
in vec3 fragmentPosition;

out vec4 out_color;

vec3 applyLight(Light light, vec3 surfaceColor, vec3 surfaceNormal, vec3 surfacePosition) {
  vec3 surfaceToLight;
  vec3 surfaceToCamera = normalize(cameraPosition - surfacePosition);
  float attenuation = 1.0;

  if(light.position.w == 0.0) {
    surfaceToLight = normalize(light.position.xyz);
  } else {
    surfaceToLight = normalize(light.position.xyz - surfacePosition);
    float distanceToLight = length(light.position.xyz - surfacePosition);
    attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));
  }

  // Calculate ambient component
  vec3 ambient = light.ambientStrength * surfaceColor * light.color;

  // Calculate diffuse component
  float diffuseStrength = max(dot(surfaceNormal, surfaceToLight), 0.0);
  vec3 diffuse = diffuseStrength * light.color * surfaceColor;

  // Calculate specular component
  float specularStrength = 0.0;
  float shininess = 80.0f;
  if(diffuseStrength > 0.0) {
    specularStrength = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, surfaceNormal))), shininess);
  }

  vec3 specular = specularStrength * vec3(1.0f, 1.0f, 1.0f) * light.color;

  return ambient + attenuation*(diffuse+specular);
}

void main(){
    // Find normal of fragment in world space
    vec3 normal = normalize(transpose(inverse(mat3(model))) * fragmentNormal);
    vec3 position = vec3(model * vec4(fragmentPosition, 1));

    vec3 color = vec3(0);

    for(int i = 0; i < numLights; i++) {
      color += applyLight(lights[i], fragmentColor, normal, position);
    }

    out_color = vec4(color, 1.0);
}
