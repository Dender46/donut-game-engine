#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Donut {

	class DONUT_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define DN_CORE_TRACE(...)     ::Donut::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DN_CORE_INFO(...)      ::Donut::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DN_CORE_WARN(...)      ::Donut::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DN_CORE_ERROR(...)     ::Donut::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DN_CORE_FATAL(...)     ::Donut::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define DN_TRACE(...)		   ::Donut::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DN_INFO(...)		   ::Donut::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DN_WARN(...)		   ::Donut::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DN_ERROR(...)		   ::Donut::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DN_FATAL(...)		   ::Donut::Log::GetCoreLogger()->fatal(__VA_ARGS__)