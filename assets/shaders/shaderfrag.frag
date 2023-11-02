
#version 450

//layout(binding = 4) uniform sampler2D texSampler;

layout(set = 1, binding = 4) uniform sampler2D texArraySampler[2000];

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragPosWorld;
layout(location = 3) in vec3 fragNormalWorld;  

layout(location = 0) out vec4 outColor;

struct PointLight{
    vec4 Position;
    vec4 Color;
    vec4 pad[2];
};

layout(set = 0, binding = 0) uniform GlobalUBO{
    mat4 proj;
    mat4 view;
    //mat4 InvView;
    vec4 ambientlightcolor;
    PointLight pointlights[10];
    int activelights;
    vec4 pad[15];    
}PL;

/*
    layout(std140, binding = 3) uniform texArrayUBO
    {
        vec4 m_TextureID;
    }textureArrUniform;
*/


//updated per frame per model
layout(push_constant) uniform Push {
    mat4 modelMatrix;
    mat4 normalMatrix;
    uint textureIndex;
    float padding[3]; 
} push;


void main() {
    // vec3 directiontolight = PL.pointlights[0].Position.xyz - fragPosWorld;
    // float atten = 1.0/dot(directiontolight, directiontolight);

    // vec3 lightcolor = PL.pointlights[0].Color.xyz * PL.pointlights[0].Color.w * atten;
    // vec3 ambientlight = PL.ambientlightcolor.xyz * PL.ambientlightcolor.w;
    // vec3 diffuselight = lightcolor * max(dot(normalize(fragNormalWorld),normalize(directiontolight)),0);


    // outColor = vec4((diffuselight*ambientlight)*fragColor,1.0);
    int textureID = int(push.textureIndex);
    vec3 ambientlight = PL.ambientlightcolor.xyz * PL.ambientlightcolor.w;
    vec3 SurfaceNormal = normalize(fragNormalWorld);
    vec3 diffuselight = vec3(0.0,0.0,0.0);
    for (int i=0; i<PL.activelights; ++i){
        PointLight currentlight = PL.pointlights[i];
        vec3 directiontolight = PL.pointlights[i].Position.xyz - fragPosWorld;
        float atten = 1.0/dot(directiontolight, directiontolight);
        directiontolight = normalize(directiontolight);
        
        float cosAngleIncidence = max(dot(SurfaceNormal, directiontolight),0);
        vec3 intensity = currentlight.Color.xyz + currentlight.Color.w * atten;
        
        diffuselight += intensity * cosAngleIncidence;
    }
    
    vec4 texColor = texture(texArraySampler[textureID], fragTexCoord);
    //vec4 texColor = texture(texArraySampler, fragTexCoord);
    //outColor = vec4((diffuselight*ambientlight)*fragColor,1.0);
    //outColor = texture(texSampler, fragTexCoord);
    outColor = texColor * vec4((diffuselight + ambientlight), 1.0);
}