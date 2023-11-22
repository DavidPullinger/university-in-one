#version 330 core
in vec4 vertPosition, vertNormal, vertTangent, vertBitangent;
in vec2 vertTexCoord;

out vec3 fragPosition, fragNormal, fragTangent, fragBitangent;
out vec2 fragTexCoord;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
void main()
{
  // transform vectors into camera space
  fragPosition = vec3(modelViewMatrix*vertPosition);
  fragNormal = normalMatrix*vertNormal.xyz;
  fragTangent = normalMatrix*vertTangent.xyz;
  fragBitangent = normalMatrix*vertBitangent.xyz;

  fragTexCoord = vertTexCoord;
  
  gl_Position = projectionMatrix*vec4(fragPosition, 1.0);
}
