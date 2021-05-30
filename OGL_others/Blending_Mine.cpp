// first_OpenGL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <filesystem>
#include "pch.h"
#include <conio.h>
#include <iostream>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <map>
#include <iterator>

using namespace std;

struct shaderProgramReturnType
{
	string vertexSource;
	string fragmentSource;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
static shaderProgramReturnType ParseShader(const string &filePath);
static unsigned int createShaders(const string &vertexSource, const string &fragmentSource);
unsigned int loadTexture(char const * path);


const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTex;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"out vec3 OurColor;"
"out vec2 Texcoord;\n"
"void main()\n"
"{\n"
"	gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"	OurColor = aColor;\n"
"	Texcoord = aTex;\n"
"}\n";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform sampler2D ourTexture;\n"
"in vec3 OurColor;\n"
"in vec2 Texcoord;\n"
"void main()\n"
"{\n"
"	vec4 texColor = texture(ourTexture, Texcoord);\n"
//"	if(texColor.a < 0.1)\n"
//"	{\n"
//"		discard;\n"
//"	}\n"
"	FragColor = texColor;\n"
"}\n";

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//camera attributes
glm::vec3 cameraPos(0.0, 0.0, 3.0);
glm::vec3 cameraFront(0.0, 0.0, -1.0);
glm::vec3 cameraUp(0.0, 1.0, 0.0);

float deltaTime = 0.0f;
float lastFrame = 0.0f;


int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Setting Up", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	/*shaderProgramReturnType source = ParseShader("res/shader/Basic.shader");
	cout << source.vertexSource;
	unsigned int shaderProgram = createShaders(source.vertexSource, source.fragmentSource);*/
	
	unsigned int shaderProgram = createShaders(vertexShaderSource, fragmentShaderSource);

	float vertices[] =
	{	// positions	  // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	float squareVertices[] = 
	{
	   // positions      // texture Coords (swapped y coordinates because texture is flipped upside down)
	   0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
	   0.0f, -0.5f, 0.0f, 0.0f, 1.0f,
	   1.0f, -0.5f, 0.0f, 1.0f, 1.0f,

	   0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
	   1.0f, -0.5f, 0.0f, 1.0f, 1.0f,
	   1.0f, 0.5f, 0.0f, 1.0f, 0.0f
	};


	//Multi-color coordinates, if you want add them, kinda makes it fancy
	/*
	1.0, 0.0, 0.0
	0.0, 1.0, 0.0
	0.0, 0.0, 1.0
	1.0, 0.0, 0.0
	0.0, 0.0, 1.0
	0.0, 1.0, 0.0
	*/

	glm::vec3 Mah_Posi[] =
	{
		glm::vec3(0.0f,  -0.01f, -7.0f),
		glm::vec3(0.6f, -0.01f , -10.0f),
		glm::vec3(-1.5f, -0.01f, -4.0f),
		glm::vec3(-0.8f, -0.01f, -12.3f),
		glm::vec3(1.4f, -0.01f , -3.5f),
	};


	glm::vec3 Mah_Posi_grass[] =
	{
		glm::vec3(-0.5f, -0.01f, -6.2f),
		glm::vec3(0.1f, -0.01f , -9.2f),
		glm::vec3(-2.0f, -0.01f , - 3.2f),
		glm::vec3(-1.3f, -0.01f , -11.4f),
		glm::vec3(0.9f, -0.01f , -2.6f),
		glm::vec3(0.1f, 0.1f , -0.6f),
		glm::vec3(-0.1f, 0.25f , -2.6f)
	};


	float planeVertices[] = 
	{
		// positions          // texture Coords 
		 5.0f, -0.8f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.8f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.8f, -5.0f,  0.0f, 2.0f,

		 5.0f, -0.8f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.8f, -5.0f,  0.0f, 2.0f,
		 5.0f, -0.8f, -5.0f,  2.0f, 2.0f
	};

	//Sorting the position of the transparent objects to make the job the depth buffer a little more easy 
	std::map<float, glm::vec3> sorted;
	for (unsigned int i = 0 ; i < (sizeof(Mah_Posi_grass) / 12) ; i++)
	{
		float distance = glm::length(glm::vec3(0.0, 0.0, -5.0) - Mah_Posi_grass[i]);
		sorted[distance] = Mah_Posi_grass[i];
	}

	//glm::vec3 l = glm::vec3(1.0, 1.0, 1.0);
	//cout << "length = " << sizeof(l) << endl;


	//setting up the buffer objects
	unsigned int cubeVBO, cubeVAO;

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 5 * sizeof(float), (void *)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);


	unsigned int transparentVBO, transparentVAO;

	glGenVertexArrays(1, &transparentVAO);
	glGenBuffers(1, &transparentVBO);

	glBindVertexArray(transparentVAO);
	glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);


	unsigned int planeVBO, planeVAO;

	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);

	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);


	string path_cube = "box.jpg";
	const char *cubePath = path_cube.c_str();
	unsigned int texture = loadTexture(cubePath);

	string path_grass = "semi_transparent.png";
	const char *grassPath = path_grass.c_str();
	unsigned int grassTexture = loadTexture(grassPath);

	string path_plane = "plane.jpg";
	const char *planePath = path_plane.c_str();
	unsigned int planeTexture = loadTexture(planePath);

	glUseProgram(shaderProgram);
	glUniform1f(glGetUniformLocation(shaderProgram, "ourTexture"), 0);


	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2, 0.4, 0.4, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		//using shader
		glUseProgram(shaderProgram);

		//View matrix
		glm::mat4 view;
		float radius = 20.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		view = glm::translate(view, glm::vec3(0.0, 0.0, -3.4));
		//view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		//Cube
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		for (int i = 0; i < 5; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, Mah_Posi[i]);
			float angle = 10.0f * i + 10.0f;		//May 21.0f is a little too much here, huh?
			//model = glm::rotate(model, float(glfwGetTime() * angle), glm::vec3(1.0, 0.3, 0.5));		//glm::radians(float(glfwGetTime() * 50)),
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//FLOOR
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, planeTexture);

		glm::mat4 model;
		//model = glm::translate(model, );
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Transparent
		glBindVertexArray(transparentVAO);
		glBindTexture(GL_TEXTURE_2D, grassTexture);

		for (int i = 0; i < 7; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, Mah_Posi_grass[i]);
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}


		/*for (std::map<float, glm::vec3>::reverse_iterator it  = sorted.rbegin ; it != sorted.rend ; ++it)
		{
			glm::mat4 model;
			model = glm::translate(model, it->second);
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}*/
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


void processInput(GLFWwindow *window)
{
	float cameraSpeed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPos += cameraFront * cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPos -= cameraFront * cameraSpeed;

	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp) * cameraSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp) * cameraSpeed);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static shaderProgramReturnType ParseShader(const string &filePath)
{
	ifstream stream(filePath);
	enum class shaderType
	{
		NONE = -1, VERTEX = 0, FRAGMMENT = 1

	};

	string line;
	stringstream ss[2];
	shaderType type = shaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != string::npos)
		{
			if (line.find("vertex") != string::npos)
			{
				type = shaderType::VERTEX;
			}

			else if (line.find("fragment") != string::npos)
			{
				type = shaderType::FRAGMMENT;
			}
		}

		else
		{
			ss[(int)type] << line << '\n';
		}

		return {ss[0].str(), ss[1].str()};
	}
}

static unsigned int createShaders(const string &vertexSource, const string &fragmentSource)
{
	//vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vs_src = vertexSource.c_str();
	glShaderSource(vertexShader, 1, &vs_src, nullptr);
	glCompileShader(vertexShader);
	//here you can check for any compilation errors but that isn't very necessary

	//fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fs_src = fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &fs_src, nullptr);
	glCompileShader(fragmentShader);
	//here too you can check for errors

	//link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	return shaderProgram;
}

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}