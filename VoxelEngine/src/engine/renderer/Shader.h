#pragma once

#include <glm/glm.hpp>
#include <iostream>

namespace VoxelEngine {
	struct ShaderProgramSource
	{
		std::string VertSrc;
		std::string FragSrc;
	};

	ShaderProgramSource ParseShader(const std::string& filepath);

    class Shader
    {
	public:
		Shader(const std::string& shaderFilepath);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetInt(const std::string& name, int value);
		void SetIntArray(const std::string& name, int* values, uint32_t count);
		void SetFloat(const std::string& name, float value);
		void SetFloat2(const std::string& name, const glm::vec2& value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetFloat4(const std::string& name, const glm::vec4& value);
		void SetMat4(const std::string& name, const glm::mat4& value);

		uint32_t GetRendererID() const { return m_RendererID; }

	private:
		void compileErrors(uint32_t shader, const std::string& type);
	private:
		uint32_t m_RendererID;

		friend class ShaderStorageBlock;
    };
}