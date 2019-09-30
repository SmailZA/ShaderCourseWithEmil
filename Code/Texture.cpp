#include "Texture.h"
#include <SOIL.h>
#include <cstring>

void SwizzleRows(unsigned char* Data, int Width, int Height)
{

	unsigned char* Temp = new unsigned char[Width * 4];
	int NumSwaps = Height / 2;

	for (int i=0; i < NumSwaps; i++)
	{

		unsigned char* SourcePtr = Data + (Width * 4) * i;
		unsigned char* TargetPtr = Data + (Width * 4) * (Height - i - 1);

		/*for (int b = 0; b < Width * 4; b++)
		{
		Temp[b] = SourcePtr[b];
		}
*/
		//Step1 Copy Source row to Temp buffer
		memcpy(Temp, SourcePtr, Width * 4);

		//Step2 Target row to Source row
		memcpy(SourcePtr, TargetPtr, Width * 4);

		//Step3 Temp buffer to Target row
		memcpy(TargetPtr, Temp, Width * 4);
	}

	delete[] Temp;

}


void Texture::LoadFile(const char* File)
{
	int Width;
	int Height;
	int Channels;
	unsigned char* ImagePixels = SOIL_load_image(File, &Width, &Height, &Channels, SOIL_LOAD_RGBA);
	SwizzleRows(ImagePixels, Width, Height);

	GLuint Texture;
	glGenTextures(1, &Handle);
	glBindTexture(GL_TEXTURE_2D, Handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImagePixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glActiveTexture(GL_TEXTURE1);

	SOIL_free_image_data(ImagePixels);
}

void Texture::Bind(int Index)
{
	glActiveTexture(GL_TEXTURE0 + Index);
	glBindTexture(GL_TEXTURE_2D, Handle);

}
