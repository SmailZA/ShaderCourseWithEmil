#include "Material.h"
#include <stdio.h>
#include <glm/glm/gtc/type_ptr.hpp>

GLuint LoadShader(const char* Path, GLenum ShaderType)
{
	
	GLuint ShaderHandle = glCreateShader(ShaderType);

	FILE* File;
	fopen_s(&File, Path, "rb");

	if (File == nullptr)
	{
		printf("Shader file '%s' does not exist! \n", Path);
		return ShaderHandle;
	}

	fseek(File, 0, SEEK_END);
	int FileLength = ftell(File);
	char* FileContents = new char[FileLength];
	fseek(File, 0, SEEK_SET);

	fread(FileContents, 1, FileLength, File);
	
	fclose(File);

	glShaderSource(ShaderHandle, 1, &FileContents, &FileLength);
	glCompileShader(ShaderHandle);

	delete[] FileContents;

	return ShaderHandle;

}

void Material::LoadFiles(const char* VertexPath, const char* FragmentPath)
{
	GLuint VertexHandle = LoadShader(VertexPath, GL_VERTEX_SHADER);
	GLuint FragmentHandle = LoadShader(FragmentPath, GL_FRAGMENT_SHADER);

	ProgramHandle = glCreateProgram();
	glAttachShader(ProgramHandle, VertexHandle);
	glAttachShader(ProgramHandle, FragmentHandle);
	glLinkProgram(ProgramHandle);

	glDetachShader(ProgramHandle, VertexHandle);
	glDetachShader(ProgramHandle, FragmentHandle);
	glDeleteShader(VertexHandle);
	glDeleteShader(FragmentHandle);

	//Error checking
	GLint LinkStatus;
	glGetProgramiv(ProgramHandle, GL_LINK_STATUS, &LinkStatus);

	if (LinkStatus == GL_FALSE)
	{
		printf("Program faileed to link \n");

		char LogBuffer[1024];
		glGetProgramInfoLog(ProgramHandle, 1024, nullptr, LogBuffer);

		printf("%s\n", LogBuffer);
	}
}

void Material::Use() const
{
	glUseProgram(ProgramHandle);
}

void Material::Set(const char* Name, const glm::mat4& Value) const
{
	Use();
	GLuint Location = glGetUniformLocation(ProgramHandle, Name);
	glUniformMatrix4fv(Location, 1, false, glm::value_ptr(Value));
}

void Material::Set(const char* Name, const glm::vec3& Value)const
{

	Use();
	GLuint Location = glGetUniformLocation(ProgramHandle, Name);
	glUniform3fv(Location, 1, glm::value_ptr(Value));

}

void Material::Set(const char* Name, const int Value)const
{
	Use();
	GLuint Location = glGetUniformLocation(ProgramHandle, Name);
	glUniform1i(Location, Value);

}

void Material::Set(const char* Name, const float Value)const
{
	Use();
	GLuint Location = glGetUniformLocation(ProgramHandle, Name);
	glUniform1f(Location, Value);


	Use();
	GLuint ResetFrameBuffer = glGetUniformLocation(ProgramHandle, Name);
	glUniform2f(ResetFrameBuffer, 1, Value);
}


