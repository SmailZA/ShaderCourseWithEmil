#version 330 core

uniform samplerCube u_Sampler;

out vec4 o_Color;

in vec3 f_CubeDirection;

void main()
{
	
	o_Color = texture(u_Sampler, f_CubeDirection);

}