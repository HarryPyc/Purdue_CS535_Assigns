#version 450
struct PointLight{
	vec3 color;
	vec3 pos;
};

out vec4 fragcolor;   

uniform vec3 camPos;
uniform PointLight light;
uniform sampler2D Tex; 

in vec3 pos;
in vec3 normal;
in vec2 uv;

float attenuation(float d){ return 1.f/(1.f+0.1f*d+d*d); }
void main(void)
{   
	float Ka = 0.2f, Kd = 1.0f, Ks = 2.0f;
	vec3 l = light.pos - pos;
	float d = length(l); float att = attenuation(d);
	l = normalize(l);

	vec3 v = normalize(camPos - pos), r = reflect(-l, normal);
	vec3 color = vec3(texture2D(Tex, uv));
	color *= light.color*(Ka + att * (Kd*max(0.f, dot(normal,l)) + Ks*pow(max(0.f, dot(r,v)), 10.f)));
	fragcolor = vec4(color, 1.0);
}




















