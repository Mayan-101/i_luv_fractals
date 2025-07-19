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
	layout (location = 1) in vec2 aTex;
	uniform vec2 resolution;
	out vec2 texCoord;
	void main()
	{	
		vec2 res_vec = aPos.xy / resolution * 2.0f;
		gl_Position = vec4(res_vec, aPos.z, 1.0);
		texCoord = aTex;
		
	}
)";

	const char* fragmentShaderSource = R"(
	#version 460 core
	uniform sampler2D tex0;
	in vec2 texCoord;
	out vec4 FragColor;
	void main() {
		
		FragColor = texture(tex0, texCoord);

	}
)";

	const char*  computeShaderSource = R"(
		#version 460 core
		layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
		layout(rgba32f, binding = 0) writeonly uniform image2D outputTexture;
		uniform vec2 resolution;
		uniform float time;
		void main(){
			ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);
			vec2 z = ((2.0f * pixelCoord.xy / resolution) - vec2(1.0f, 1.0f))*2.5f;
			vec2 c = vec2(0.5f*sin(0.3f*time) , 0.75f*cos(0.3f*time)) ;
			vec3 hot = vec3(0.0f, 0.0f, 0.0f);
			vec3 mid1 = vec3(0.0f, 0.0f, 1.0f);
			vec3 mid2 = vec3(0.0f, 1.0f, 0.0f);
			vec3 mid3 = vec3(1.0f, 0.0f, 0.0f);
			vec3 cold = vec3(0.0f, 1.0f, 1.0f);
			vec3 color;
			int max_iterations = 256;
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

			imageStore(outputTexture, pixelCoord, vec4(color, 1.0f));
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
		-(float)resolution[0]/2.0f, (float)resolution[1], 0.0f,		0.0f, 0.0f,
		-(float)resolution[0]/2.0f, -(float)resolution[1], 0.0f,		0.0f, 1.0f, 
		(float)resolution[0]/2.0f, -(float)resolution[1], 0.0f,		1.0f, 1.0f,
		(float)resolution[0]/2.0f, (float)resolution[1], 0.0f,		1.0f, 0.0f

	};

	GLuint indices[] = {
		0, 1, 2,
		0, 3, 2
	};

	int heightImg = resolution[1];
	int widthImg = resolution[0];
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, widthImg, heightImg, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeShader, 1, &computeShaderSource, NULL);
    glCompileShader(computeShader);
	GLuint computeProgram = glCreateProgram();
	glAttachShader(computeProgram, computeShader);
	glLinkProgram(computeProgram);
	

	Shader shaderProgram(vertexShaderSource, fragmentShaderSource);
	VAO VAO1;
	
	VAO1.Bind();
	
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3*sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	GLint res_vec_loc1 = glGetUniformLocation(shaderProgram.ID, "resolution");
	GLint res_vec_loc2 = glGetUniformLocation(computeProgram, "resolution");
	GLint time_loc = glGetUniformLocation(computeProgram, "time");
	GLint tex_loc = glGetUniformLocation(shaderProgram.ID, "tex0");
	shaderProgram.Activate();
	glUniform1i(tex_loc, 0);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		
		glUseProgram(computeProgram);
		glUniform2f(res_vec_loc2, (float)resolution[0], (float)resolution[1]);
        glUniform1f(time_loc, (float)glfwGetTime());
		glDispatchCompute((GLuint)ceil(widthImg / 16.0f), (GLuint)ceil(heightImg / 16.0f), 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		

		shaderProgram.Activate();
		glUniform2f(res_vec_loc1, (float)resolution[0], (float)resolution[1]);
		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
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
	glDeleteShader(computeShader);
	glDeleteTextures(1, &texture);
	glDeleteProgram(computeProgram);


	//literally destroys the window buffer
	glfwDestroyWindow(window);
	glfwTerminate();

	std::cout << "done!" <<std::endl;
	return 0;
}
