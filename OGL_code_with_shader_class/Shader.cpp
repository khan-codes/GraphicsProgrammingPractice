#include "Shader.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glad\glad.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

using namespace std;

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
{
	//READING FILES


	ifstream Vstream(vertexPath);
	ifstream Fstream(fragmentPath);
	stringstream ss[3];
	string Vline, Fline, Vsource, Fsource;
	

	
	//for vertex shader
	while (getline(Vstream, Vline))
	{
		ss[1] << Vline << '\n';
	}

	//cout << "Vertex\n" << ss[1].str() << "\n";
	
	Vsource = ss[1].str();

	//for fragment shader
	while (getline(Fstream, Fline))
	{
		ss[2] << Fline << '\n';
	}

	//cout << "Fragment\n" <<ss[2].str() << "\n";
	Fsource = ss[2].str();


	//CREATING SHADER PROGRAM
	//vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vs_src = Vsource.c_str();

	glShaderSource(vertexShader, 1, &vs_src, nullptr);
	glCompileShader(vertexShader);
	//here you can check for any compilation errors but that isn't very necessary

	//fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fs_src = Fsource.c_str();
	glShaderSource(fragmentShader, 1, &fs_src, nullptr);
	glCompileShader(fragmentShader);
	//here too you can check for errors

	//link shaders
	Shader::ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	//try using this pointer here
}

unsigned int Shader::getID()
{
	return ID;
}

void Shader::use()
{
	glUseProgram(ID);
}

// utility uniform functions

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}


void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}



void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string &name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


//ERROR CHECKING
void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}
