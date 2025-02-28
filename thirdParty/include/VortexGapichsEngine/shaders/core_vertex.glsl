#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
//layout (location = 3) in mat4 boneMatrices[100];

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
    //mat4 skinningMatrix = mat4(0.0f);
    //    for (int i = 0; i < 100; i++) {
    //    skinningMatrix += boneMatrices[i] * (aPos.x == i ? 1.0f : 0.0f);
    //}
    //vec4 skinnedPosition = skinningMatrix * vec4(aPosition, 1.0f);

    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
    vs_out.Position = vec3(model * vec4(aPos, 1.0f));
    vs_out.TexCoord = aTexCoord;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.Position, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}
