#version 330

	out vec4 o_Color;

	in vec2 blurTextureCoords[11];

	uniform sampler2D originalTexture;

	void main()
	{
		o_Color = vec4(1.0, 0.2, 0.1, 1.0);
		o_Color += texture(originalTexture, blurTextureCoords[0]) * 0.0093;
		o_Color += texture(originalTexture, blurTextureCoords[1]) * 0.028002;
		o_Color += texture(originalTexture, blurTextureCoords[2]) * 0.065984;
		o_Color += texture(originalTexture, blurTextureCoords[3]) * 0.021703;
		o_Color += texture(originalTexture, blurTextureCoords[4]) * 0.075713;
		o_Color += texture(originalTexture, blurTextureCoords[5]) * 0.198596;
		o_Color += texture(originalTexture, blurTextureCoords[6]) * 0.075713;
		o_Color += texture(originalTexture, blurTextureCoords[7]) * 0.021703;
		o_Color += texture(originalTexture, blurTextureCoords[8]) * 0.065984;
		o_Color += texture(originalTexture, blurTextureCoords[9]) * 0.028002;
		o_Color += texture(originalTexture, blurTextureCoords[10]) * 0.0093;

	}