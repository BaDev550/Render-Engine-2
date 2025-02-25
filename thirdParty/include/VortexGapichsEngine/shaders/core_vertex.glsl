#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out VS_OUT{
    vec3 Position;
    vec2 TexCoord;
    vec3 Normal;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 model;
uniform mat4 lightSpaceMatrix;

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};

void main() {
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
    vs_out.Position = vec3(model * vec4(aPos, 1.0));
    vs_out.TexCoord = aTexCoord;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.Position, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
