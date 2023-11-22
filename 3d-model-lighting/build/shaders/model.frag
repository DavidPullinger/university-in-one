#version 330 core
in vec3 fragPosition, fragNormal, fragTangent, fragBitangent;
in vec2 fragTexCoord;

out vec4 fragColor;

struct Light {
  vec4 position;
  vec4 ambientColor;
  vec4 diffuseColor;
  vec4 specularColor;
};
const int numLights = 3;

uniform Light lights[numLights];
uniform sampler2D albedoTex;
uniform sampler2D normalTex;
uniform float ka, kd, ks, shininess;
uniform bool useNormalMap;
void main()
{
  // Use normal map to perturb normals
  mat3 TBN = mat3(normalize(fragTangent), normalize(fragBitangent), normalize(fragNormal));
  vec3 normalMap = texture(normalTex, fragTexCoord).rgb;
  vec3 normal;
  if (useNormalMap)
  {
    normal = normalize(TBN * (normalMap * 2.0 - 1.0));
  }
  else
  {
    normal = normalize(fragNormal);
  }

  // Accumulate lighting contributions from each light source
  vec4 result = vec4(0.0);
  for (int i = 0; i < numLights; i++)
  {
    // Calculate N,L,E,H vectors
    vec3 N = normalize(normal);
    vec3 L = normalize(vec3(lights[i].position) - fragPosition);
    vec3 E = normalize(-fragPosition);
    vec3 H = normalize(L + E);

    // Ambient lighting
    float ambientStrength = ka;
    vec4 ambient = ambientStrength * lights[i].ambientColor*texture(albedoTex, fragTexCoord);

    // Diffuse lighting
    float diffuseStrength = max(dot(L, N), 0.0) * kd;
    vec4 diffuse = diffuseStrength * lights[i].diffuseColor*texture(albedoTex, fragTexCoord);

    // Specular lighting
    float specularStrength = pow(max(dot(N, H), 0.0), shininess) * ks;
    vec4 specular = specularStrength * lights[i].specularColor;

    // Accumulate
    result += ambient + diffuse + specular;
  }

  // Final color
  fragColor = vec4(result.rgb, 1.0);
}

