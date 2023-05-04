#pragma once

#include "engine/core/Base.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)


namespace VoxelEngine {
	class Log
	{
	public:
		static void init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

//Core macros
#define ENGINE_CORE_TRACE(...)	::VoxelEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ENGINE_CORE_INFO(...)	::VoxelEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ENGINE_CORE_WARN(...)	::VoxelEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ENGINE_CORE_ERROR(...)	::VoxelEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ENGINE_CORE_FATAL(...)	::VoxelEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define ENGINE_CORE_CRITICAL(...)	::VoxelEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client macros
#define ENGINE_TRACE(...)		::VoxelEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ENGINE_INFO(...)		::VoxelEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ENGINE_WARN(...)		::VoxelEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ENGINE_ERROR(...)		::VoxelEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ENGINE_FATAL(...)		::VoxelEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__)