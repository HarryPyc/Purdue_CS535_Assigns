#version 450            
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

layout(location = 0)in vec3 pos_attrib; 
layout(location = 1)in vec3 normal_attrib; 
layout(location = 2)in vec2 texCoord_attrib; 
out vec3 pos;
out vec3 normal;
out vec2 uv;
void main(void)
{

   gl_Position = vec4(pos_attrib, 1.0)*M*V*P;     //w = 1 becase this is a point
   pos = vec3(vec4(pos_attrib, 1.0)*M);
   normal = normalize(vec3( vec4(normal_attrib, 0.0f)*transpose(inverse(M))));
   uv = texCoord_attrib;
}
