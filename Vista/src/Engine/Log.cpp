#include "Log.h"

namespace vista
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::initLog()
	{
		spdlog::set_pattern("%^[%T] %n :: %v%$");

		s_CoreLogger = spdlog::stdout_color_mt("VISTA");
		s_CoreLogger->set_level(spdlog::level::trace);
		s_ClientLogger = spdlog::stderr_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}