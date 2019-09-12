#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 Tex;
layout (location = 2) in vec3 NormIn;

out vec3 normVec;
out vec3 normLightingVec;
out vec2 TexCoord;
out vec3 pos;

uniform mat4 projMatrix;
uniform mat4 transMatrix;
uniform mat4 rotationMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPos;


void main(){
    mat4 transformMat = viewMatrix * transMatrix * rotationMatrix;


    vec3 tranformedLightPos = (viewMatrix * vec4(lightPos, 1.0)).xyz;

    vec4 transformedPos = transformMat*vec4(aPos, 1.0);
    gl_Position = projMatrix * transformedPos;
    pos = transformedPos.xyz;
    vec4 transformedNorm = transformMat*vec4(NormIn, 0.0);
    normVec = transformedNorm.xyz;
    normLightingVec = (tranformedLightPos - transformedPos.xyz);
    TexCoord = Tex;
}
