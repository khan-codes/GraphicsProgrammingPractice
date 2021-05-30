// OpenGL_first.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <iostream>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 OurColor;"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos, 1.0);"
"	OurColor = aColor;"
"}\n";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 OurColor;\n"
"uniform vec3 ourColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(ourColor, 1.0f);\n"
"}\n";


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "alpha72784", NULL, NULL);
	
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers	|| THIS IS A VERY IMPORTANT CALL!!!
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	cout << "ALPHA72784" << endl;



							//shaders come right here!!

	//vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
			//here you can check for any compilation errors but that isn't very necessary

	//fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
			//here too you can check for errors

	//link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	//delete the indivisual shaders as they are now linked in the shader program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	float vertices[] = 
	{	//position		    //color
		0.0f,  0.25f, 0.0f, 1.0, 0.0, 0.0, 
		0.5f, -0.5f, 0.0f, 0.0, 1.0, 0.0,
		-0.5f, -0.5f, 0.0f, 0.0, 0.0, 1.0,
	};


	unsigned int VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(float), (void *)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2, 0.4, 0.6, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		//handling the uniform
		int vertexlocation = glGetUniformLocation(shaderProgram, "ourColor");
		float timevalue = glfwGetTime();
		float greenvalue = (sin(timevalue) / 2.0f) + 0.5;
		glUniform3f(vertexlocation, 0.0, greenvalue, 0.0);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

