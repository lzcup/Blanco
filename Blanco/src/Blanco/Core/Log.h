#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"

namespace Blanco 
{
	class Log{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; };
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; };
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//log macro
#define BL_CORE_TRACE(...)     Blanco::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define BL_CORE_INFO(...)      Blanco::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BL_CORE_WARN(...)      Blanco::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BL_CORE_ERROR(...)     Blanco::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BL_CORE_CRITICAL(...)  Blanco::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define BL_TRACE(...)     Blanco::Log::GetClientLogger()->trace(__VA_ARGS__)
#define BL_INFO(...)      Blanco::Log::GetClientLogger()->info(__VA_ARGS__)
#define BL_WARN(...)      Blanco::Log::GetClientLogger()->warn(__VA_ARGS__)
#define BL_ERROR(...)     Blanco::Log::GetClientLogger()->error(__VA_ARGS__)
#define BL_CRITICAL(...)  Blanco::Log::GetClientLogger()->critical(__VA_ARGS__)


