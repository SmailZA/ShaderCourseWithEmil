#version 330 core

uniform sampler2D u_FrameColor;
uniform sampler2D u_FrameDepth;

in vec2 f_TexCoord;

out vec4 o_Color;

const vec3 FogColor = vec3(0.9, 0.2, 0.2);

void main()
{
	vec4 Color = texture(u_FrameColor, f_TexCoord);
	//float Depth = texture(u_FrameDepth, f_TexCoord).x;
	//Depth = pow(Depth, 35.0) * 20.7;
	
	//Color.xyz = mix(Color.xyz, FogColor, Depth);
	o_Color = Color;
}