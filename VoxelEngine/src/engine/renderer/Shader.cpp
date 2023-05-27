#include "Shader.h"

#include "engine/core/Log.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

namespace VoxelEngine {
	ShaderProgramSource ParseShader(const std::string& filepath)
	{

		std::ifstream stream(filepath);

		enum class ShaderType
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;

		while (getline(stream, line))
		{
			if (line.find("#type") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
					type = ShaderType::VERTEX;
				else if (line.find("fragment") != std::string::npos)
					type = ShaderType::FRAGMENT;
			}
			else
			{
				ss[(int)type] << line << "\n";
			}
		}

		//ENGINE_TRACE(ss[0].str());
		//ENGINE_TRACE(ss[1].str());
		return { ss[0].str(), ss[1].str() };
	}

	Shader::Shader(const std::string& shaderFilepath)
	{
		auto& shaderSrc = ParseShader(shaderFilepath);

		std::string vertexCode = shaderSrc.VertSrc;
		std::string fragmentCode = shaderSrc.FragSrc;

		const char* vertexSource = vertexCode.c_str();
		const char* fragmentSource = fragmentCode.c_str();

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);
		compileErrors(vertexShader, "VERTEX");

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);
		compileErrors(fragmentShader, "FRAGMENT");

		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);
		glLinkProgram(m_RendererID);
		compileErrors(m_RendererID, "PROGRAM");

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetInt(const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
	}

	void Shader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		glUniform1iv(glGetUniformLocation(m_RendererID, name.c_str()), count, values);

	}

	void Shader::SetFloat(const std::string& name, float value)
	{
		glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
	}

	void Shader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		glUniform2f(glGetUniformLocation(m_RendererID, name.c_str()), value.x, value.y);
	}

	void Shader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		glUniform3f(glGetUniformLocation(m_RendererID, name.c_str()), value.x, value.y, value.z);
	}

	void Shader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		glUniform4f(glGetUniformLocation(m_RendererID, name.c_str()), value.x, value.y, value.z, value.w);
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, false, glm::value_ptr(value));
	}

	void Shader::compileErrors(uint32_t shader, const std::string& type)
	{
		// Stores status of compilation
		GLint hasCompiled;
		// Character array to store error message in
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
			if (hasCompiled == GL_FALSE)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				ENGINE_CORE_ERROR("SHADER_COMPILATION_ERROR for: " + type);
				ENGINE_CORE_ERROR("SHADER_COMPILATION_ERROR for: " + std::string(infoLog));
			}
		}
		else
		{
			glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
			if (hasCompiled == GL_FALSE)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				ENGINE_CORE_ERROR("SHADER_LINKING_ERROR for: " + type);
				ENGINE_CORE_ERROR("SHADER_COMPILATION_ERROR for: " + std::string(infoLog));
			}
		}
	}
}