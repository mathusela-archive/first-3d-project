#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

out vec3 fNormal;
out vec3 fPos;
out vec2 fUV;

uniform mat4 testprojection;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

void main() {

   gl_Position = projection * view * world * vec4(vPos, 1.0);
   fPos = vec3(world * vec4(vPos, 1.0));
   fNormal = vNormal;
   fUV = vUV;

}
