#version 450

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec4 vNormals;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragPosWorld;
layout(location = 3) out vec3 fragNormalWorld;

struct PointLight{
    vec4 Position;
    vec4 Color;
    vec4 pad[2];
};

//for the scene
layout(set = 0, binding = 0) uniform GlobalUBO{
    mat4 proj;
    mat4 view;
    //mat4 InvView;
    vec4 ambientlightcolor;
    PointLight pointlights[10];
    int activelights;
    vec4 pad[15];    
}PL;



//updated per frame per model
layout(push_constant) uniform Push {
    mat4 modelMatrix;
    mat4 normalMatrix;
    uint textureIndex;
    float padding[3]; 
} push;




void main() {
    vec4 position_in_world = push.modelMatrix * vec4(vPosition, 1.0);
    gl_Position = PL.proj * PL.view *push.modelMatrix* vec4(vPosition, 1.0);
    
    fragNormalWorld = normalize(mat3(push.normalMatrix) * vNormals.xyz);
    fragPosWorld = position_in_world.xyz;

    fragColor = vColor;
    fragTexCoord = inTexCoord;
}