#pragma once
#include <GL/glew.h>
#include <glm/glm/glm.hpp>

class Material
{

public:
	void LoadFiles(const char* VertexPath, const char* FragmentPath);
	void Use() const;
	void Set(const char* Name, const glm::mat4& Value) const;
	void Set(const char* Name, const glm::vec3& Value) const;
	void Set(const char* Name, const int Value) const;
	void Set(const char* Name, const float Value) const;
	
	
	GLuint ProgramHandle;
};