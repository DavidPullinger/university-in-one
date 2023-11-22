#version 330 core
in vec4 vertPosition;
out vec3 fragTexCoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
void main()
{
    fragTexCoord = vertPosition.xyz;
    vec4 position = projectionMatrix*viewMatrix*vec4(vertPosition.xyz, 1.0);
    gl_Position = position.xyww;
}

