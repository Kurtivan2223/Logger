#include "Logs.h"

Logs::Logs()
{
	
}

Logs::~Logs()
{
	
}

Logs* Logs::Instance()
{
	std::lock_guard<std::mutex> lock(mtx);
	static Logs instance;
	
	return &instance;
}

void Logs::WriteMessage(LOG_LEVEL level, const std::string& message)
{
	std::lock_guard<std::mutex> lock(mtx);
	
	std::string _temp;
	std::string _timestamp = GetTimeStamp();
	
	switch(LOG_LEVEL)
	{
		case LOG_LEVEL::LOG_LEVEL_TRACE:
			_temp = "[TRACE] " + _timestamp + " - " + message;
			break;
		case LOG_LEVEL::LOG_LEVEL_DEBUG:
			_temp = "[DEBUG] " + _timestamp + " - " + message;
			break;
		case LOG_LEVEL::LOG_LEVEL_INFO:
			_temp = "[INFO] " + _timestamp + " - " + message;
			break;
		case LOG_LEVEL::LOG_LEVEL_WARNING:
			_temp = "[WARNING] " + _timestamp + " - " + message;
			break;
		case LOG_LEVEL::LOG_LEVEL_ERROR:
			_temp = "[ERROR] " + _timestamp + " - " + message;
			break;
		case LOG_LEVEL::LOG_LEVEL_FATAL:
			_temp = "[FATAL] " + _timestamp + " - " + message;
			break;
	}
	
	
	WriteToConsole(level, _temp);
	WriteToFile(level, _temp);
}

void Logs::WriteToConsole(LOG_LEVEL level, const std::string& message)
{
	std::lock_guard<std::mutex> lock(mtx);
	if(level == LOG_LEVEL::LOG_LEVEL_TRACE || level == LOG_LEVEL::LOG_LEVEL_DEBUG || level == LOG_LEVEL::LOG_LEVEL_INFO)
		std::cout << message << std::endl;
	
	if(level == LOG_LEVEL::LOG_LEVEL_WARNING || level == LOG_LEVEL::LOG_LEVEL_ERROR || level == LOG_LEVEL::LOG_LEVEL_FATAL)
		std::cerr << message << std::endl;
}

void Logs::WriteToFile(LOG_LEVEL level, const std::string& message)
{
	std::lock_guard<std::mutex> lock(mtx);
	
	LogFile("Logs.Log", std::ios::app);
	ErrorLogFile("Error.Log", std::ios::app);
	
	//normal logging
	if(level == LOG_LEVEL::LOG_LEVEL_TRACE || level == LOG_LEVEL::LOG_LEVEL_DEBUG || level == LOG_LEVEL::LOG_LEVEL_INFO)
	{
		if(LogFile.is_open())
		{
			LogFile << message << std::endl;
		}
	}
	
	//error logging
	if(level == LOG_LEVEL::LOG_LEVEL_WARNING || level == LOG_LEVEL::LOG_LEVEL_ERROR || level == LOG_LEVEL::LOG_LEVEL_FATAL)
	{
		if(ErrorLogFile.is_open())
		{
			ErrorLogFile << message << std::endl;
		}
	}
}

std::string Logs::GetTimeStamp()
{
	auto now = std::chrono::system_clock::now();
    std::time_t current = std::chrono::system_clock::to_time_t(now);
    std::string _temp = std::ctime(&current);
    _temp.pop_back(); // Removing the newline character at the end
    return _temp;
}

