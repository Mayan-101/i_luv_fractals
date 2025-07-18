// well .. i am not that great when it cums to comments n stuff 
// anyways it's just for my own reference
// like ... include all dependencies
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"VAO.h"
#include"EBO.h"
#include "shaderClass.h"
#include"VBO.h"


int main() {

	const char* vertexShaderSource = R"(
	#version 460 core
	layout (location = 0) in vec3 aPos;
	uniform vec2 resolution;
	out vec4 color;
	void main()
	{	
		vec2 res_vec = aPos.xy / resolution * 2.0f;
		gl_Position = vec4(res_vec, aPos.z, 1.0);
		
	}
)";

	const char* fragmentShaderSource = R"(
	#version 460 core
	uniform vec2 resolution;
	uniform float time;
	out vec4 FragColor;
	void main() {
		vec2 c = vec2(0.5f*sin(0.15f*time) , 0.75f*cos(0.25f*time)) ;
		vec2 z = (2.0f * gl_FragCoord.xy / (resolution) - vec2(1.0f, 0.6f))*2.5f;
		vec3 hot = vec3(0.0f, 0.0f, 0.0f);
		vec3 mid1 = vec3(0.0f, 0.0f, 1.0f);
		vec3 mid2 = vec3(0.0f, 1.0f, 0.0f);
		vec3 mid3 = vec3(1.0f, 0.0f, 0.0f);
		vec3 cold = vec3(0.0f, 1.0f, 1.0f);
		vec3 color;
		int max_iterations = 512;
		int iterations = 0;
		for (int i = 0; i < max_iterations; i++){
			z = vec2(z.x * z.x - z.y * z.y , 2 * z.x * z.y) + c;
			iterations++;
			if (dot(z, z) > 4) {
				break;
			}
		}
		float normalizedValue = float(iterations) / float(max_iterations);
		
		if (normalizedValue < 0.25f) {
			color = mix(cold, mid1, normalizedValue * 4.0);
		} 
		else if (normalizedValue < 0.5f) {
			color = mix(mid1, mid2, (normalizedValue - 0.25) * 4.0);
		} 
		else if (normalizedValue < 0.75f){
			color = mix(mid2, mid3, (normalizedValue - 0.5) * 4.0);
		} 
		else {
			color = mix(mid3, hot, (normalizedValue - 0.75) * 4.0);
		}

		FragColor = vec4(color, 1.0f);

	}
)";
	//init the library and give it like the version n stuff that u are using
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int resolution[] = {1920, 1080};
	//literally creates a window
	GLFWwindow* window = glfwCreateWindow(resolution[0], resolution[1], "OK BROO", NULL, NULL);
	if (window == NULL) {
		std::cout << "You failed bruv..." << std::endl;
		glfwTerminate();
		return -1;
	}

	// a bit abstract in nature but it basically meaning for now focauss here..
	glfwMakeContextCurrent(window);

	// so u use "glad" to load stuff on window for example to change the color of the window, in this case..
	gladLoadGL();
	glViewport(0, 0, resolution[0], resolution[1]);
	
	
	GLfloat vertices[] = {
		-960.0f, 1080.0f, 0.0f,		
		-960.0f, -1080.0f, 0.0f,		
		960.0f, -1080.0f, 0.0f,		
		960.0f, 1080.0f, 0.0f,		

	};

	GLuint indices[] = {
		0, 1, 2,
		0, 3, 2
	};

	Shader shaderProgram(vertexShaderSource, fragmentShaderSource);
	VAO VAO1;
	VAO1.Bind();
	
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	GLint res_vec_loc = glGetUniformLocation(shaderProgram.ID, "resolution");
	GLint time_loc = glGetUniformLocation(shaderProgram.ID, "time");
	// loop to keep the window open 
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Activate();
		glUniform2f(res_vec_loc, (float)resolution[0], (float)resolution[0]);
		glUniform1f(time_loc, (float)glfwGetTime());
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		//checks for like closing, minimizing n stuff
		glfwPollEvents();
	}
	

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();


	//literally destroys the window buffer
	glfwDestroyWindow(window);
	glfwTerminate();

	std::cout << "done!" <<std::endl;
	return 0;
}