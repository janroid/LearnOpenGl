#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad\glad.h>;

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

class Shader_m
{
public:
	unsigned int ID;

	Shader_m() {};

	Shader_m(const char* vertexPath, const char* framentPath)
	{
		unsigned int vertex = createVexShder(vertexPath);
		unsigned int fragment = createFrameShder(framentPath);

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}

	Shader_m(const char* vs, const char* fs, const char* gs) {
		unsigned int vertex = createVexShder(vs);
		unsigned int fragment = createFrameShder(fs);
		unsigned int geometry = createGHShder(gs);

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glAttachShader(ID, geometry);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		glDeleteShader(geometry);
	}

	void use() {
		glUseProgram(ID);
	}

	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(getUniformLocation(ID, name.c_str()), (int)value);
	}

	void setInt(const std::string &name, int value) const
	{
		glUniform1i(getUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(getUniformLocation(ID, name.c_str()), value);
	}

	void setMat4(const std::string name, glm::mat4 value) const
	{
		glUniformMatrix4fv(getUniformLocation(ID,name.c_str()),1,GL_FALSE,glm::value_ptr(value));
	}

	void setVec3(const std::string name, glm::vec3 value) const
	{
		glUniform3fv(getUniformLocation(ID, name.c_str()), 1, &value[0]);
	}

	void setVec3(const std::string name, float x, float y, float z) const
	{
		glUniform3f(getUniformLocation(ID, name.c_str()), x, y, z);
	}

private:

	GLint getUniformLocation(GLuint program, const GLchar *name) const {
		GLint id = glGetUniformLocation(ID, name);

		if (id == -1) {
			if (strcmp(name, "material.") == 0) {
				
			}
			else
			{
				std::cout << "ERROR::SHARE_getUniformLocation -1 :" << name << std::endl;
			}
			
		}

		return id;
	}

	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];

		if (type != "PROGRAM") {
			//��ӡ����
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHARE_COMPILATION_ERROR of type:" << type << "\n" << infoLog << "\n" << std::endl;
			}

		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_COMPILATION_ERROR of type:" << type << "\n" << infoLog << "\n" << std::endl;
			}
		}


	}

	unsigned int createFrameShder(const char* framentPath) {
		std::string fragmentCode;
		std::ifstream fShaderFile;

		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			// ���ļ�
			fShaderFile.open(framentPath);
			std::stringstream fShaderStream;

			// ��ȡ�ļ����Ļ������ݵ���������
			fShaderStream << fShaderFile.rdbuf();

			//�ر��ļ�������
			fShaderFile.close();

			//ת����������string
			fragmentCode = fShaderStream.str();

		}
		catch (const std::exception&)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}

		const char* fShaderCode = fragmentCode.c_str();

		// ������ɫ��
		unsigned int fragment;

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		return fragment;
	}

	unsigned int createVexShder(const char* vertexPath) {
		std::string vertexCode;
		std::ifstream vShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			// ���ļ�
			vShaderFile.open(vertexPath);
			std::stringstream vShaderStream;

			// ��ȡ�ļ����Ļ������ݵ���������
			vShaderStream << vShaderFile.rdbuf();

			//�ر��ļ�������
			vShaderFile.close();

			//ת����������string
			vertexCode = vShaderStream.str();

		}
		catch (const std::exception&)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();

		// ������ɫ��
		unsigned int vertex;


		//������ɫ��
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		return vertex;
	}

	unsigned int createGHShder(const char* ghPath) {
		std::string ghCode;
		std::ifstream ghShaderFile;

		ghShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			// ���ļ�
			ghShaderFile.open(ghPath);
			std::stringstream gShaderStream;

			// ��ȡ�ļ����Ļ������ݵ���������
			gShaderStream << ghShaderFile.rdbuf();

			//�ر��ļ�������
			ghShaderFile.close();

			//ת����������string
			ghCode = gShaderStream.str();

		}
		catch (const std::exception&)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}
		const char* gShaderCode = ghCode.c_str();

		// ������ɫ��
		unsigned int geometry;


		//������ɫ��
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "geometry");

		return geometry;
	}
};

#endif