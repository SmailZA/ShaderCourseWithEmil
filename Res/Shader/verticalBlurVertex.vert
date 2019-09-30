#version 330

out vec2 blurTextureCoords[11];

uniform float t_TargetHeight;

const vec2 QuadVerts[] = vec2[]
(
	vec2(-1.0, -1.0),
	vec2(1.0, -1.0),
	vec2(1.0, 1.0),
	vec2(-1.0, 1.0)
);



void main(void)
{
	vec2 b_Position = QuadVerts[gl_VertexID];
	gl_Position = vec4(b_Position, 0.0, 1.0);
	

	vec2 centerTexCoords = b_Position * 0.5 + 0.5;
	float pixelSize = 1.0 / t_TargetHeight;

	for(int i = -5; i <= 5; i++)
	{
		blurTextureCoords[i+5] = centerTexCoords + vec2(0.0, pixelSize * i);
	}


}
