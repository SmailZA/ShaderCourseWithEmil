#pragma once
#include <GL/glew.h>
#include "Texture.h"


class FrameBuffer
{
public:

	GLuint Handle;
	Texture ColorTexture;
	Texture DepthTexture;
	Texture ResetFrameBuffer;

	int Width;
	int Height;


	void Create(int InWidth, int InHeight);
	void Bind();
	void UnBind();




};