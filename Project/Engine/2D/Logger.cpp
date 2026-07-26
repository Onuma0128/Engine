#include "Logger.h"
#include <windows.h>

void NumaEngine::Logger::Log(const std::string& message)
{
	OutputDebugStringA(message.c_str());
}
