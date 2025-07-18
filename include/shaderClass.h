#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <iostream>

class Shader
{
	public:
		GLuint ID ;
		Shader(const char* vertexShaderSource, const char* fragmentShaderSource);

		void Activate();
		void Delete();

};

#endif	