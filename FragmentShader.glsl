#version 440 core
in vec3 normVec;
in vec3 normLightingVec;
in vec2 TexCoord;
in vec3 pos;


out vec4 FragColor;

uniform sampler2D ourTexture;
uniform vec3 lightColour;
uniform float specularReflectivity;
uniform float specularFalloff;

void main(){
    vec3 normnormLightingVec = normalize(normLightingVec);
    vec3 normnormVec = normalize(normVec);

    vec3 lCol = normnormVec/2 + vec3(0.5, 0.5, 0.5);


    vec4 specular =
    specularReflectivity * vec4(
        pow(max(dot(reflect(normnormLightingVec, normnormVec), normalize(pos)), 0.0), specularFalloff) * lightColour
                            , 1.0);

    vec4 diffuse = vec4(max(dot(normnormLightingVec, normnormVec), 0.2) * lightColour, 1.0);
    vec4 texSample = texture(ourTexture, TexCoord);
    FragColor = diffuse * texSample + specular * texSample;
}
