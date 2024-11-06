#pragma once


#include "spdlog/spdlog.h"
#include <spdlog/logger.h>
#include "spdlog/sinks/stdout_color_sinks.h"
#include <memory>

namespace vista
{
	class Log
	{
	public:
		static void initLog();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

#define CORE_LOG_TRACE(...)  vista::Log::getCoreLogger()->trace(__VA_ARGS__)
#define CORE_LOG_INFO(...)   vista::Log::getCoreLogger()->info(__VA_ARGS__)
#define CORE_LOG_WARN(...)   vista::Log::getCoreLogger()->warn(__VA_ARGS__)
#define CORE_LOG_ERROR(...)  vista::Log::getCoreLogger()->error(__VA_ARGS__)


#define LOG_TRACE(...)       vista::Log::getClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)        vista::Log::getClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)		 vista::Log::getClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)		 vista::Log::getClientLogger()->error(__VA_ARGS__)

