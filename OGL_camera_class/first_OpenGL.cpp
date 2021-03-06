// first_OpenGL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/config.h>

#include "pch.h"
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
#include "Shader.h"
#include "Camera.h"

using namespace std;

struct shaderProgramReturnType
{
	string Source;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const * path);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	/*shaderProgramReturnType source = ParseShader("res/shader/Basic.shader");
	cout << source.vertexSource;
	unsigned int shaderProgram = createShaders(source.vertexSource, source.fragmentSource);*/

	Shader ourShader("vertex.txt", "fragment.txt");
	unsigned int shaderProgram = ourShader.getID();

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


	/*
	//Multi-color coordinates, if you want add them, kinda makes it fancy
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


	/*
	
	//Sorting the position of the transparent objects to make the job the depth buffer a little more easy 
	//std::map<float, glm::vec3> sorted;
	//for (unsigned int i = 0 ; i < (sizeof(Mah_Posi_grass) / 12) ; i++)
	//{
		//float distance = glm::length(glm::vec3(0.0, 0.0, -5.0) - Mah_Posi_grass[i]);
		//sorted[distance] = Mah_Posi_grass[i];
	//}

	//glm::vec3 l = glm::vec3(1.0, 1.0, 1.0);
	//cout << "length = " << sizeof(l) << endl;
	
	
	*/
	

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

	//glUniform1f(glGetUniformLocation(shaderProgram, "ourTexture"), 0);
	ourShader.setInt("ourTexture", 0);


	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);
	//glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	ourShader.setMat4("projection", projection);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2, 0.4, 0.4, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		//using shader
		glUseProgram(shaderProgram);

		//View matrix
		glm::mat4 view = camera.GetViewMatrix();
		//view = glm::translate(view, glm::vec3(0.0, 0.0, -3.4));		
		//glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		ourShader.setMat4("view", view);

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
			//glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
			ourShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//FLOOR
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, planeTexture);

		glm::mat4 model;

		//glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		ourShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Transparent
		glBindVertexArray(transparentVAO);
		glBindTexture(GL_TEXTURE_2D, grassTexture);

		for (int i = 0; i < 7; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, Mah_Posi_grass[i]);
			//glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
			ourShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}		

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
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
		
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
		
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
		
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
		
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
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