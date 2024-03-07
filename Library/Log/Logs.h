#ifndef __LOGS__H__
#define __LOGS__H__

#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <mutex>
#include <chrono>
#include <ctime>

using namespace std;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

enum LOG_LEVEL : uint8 {
	LOG_LEVEL_TRACE, // 0x00
	LOG_LEVEL_DEBUG, // 0x01
	LOG_LEVEL_INFO, // 0x02
	LOG_LEVEL_WARNING, // 0x03
	LOG_LEVEL_ERROR, // 0x04
	LOG_LEVEL_FATAL // 0x05
};

class Logs
{
private:
	Logs(); // constructor
	~Logs(); // deconstructor
	
	Logs(const Logs&) = delete;
	Logs& operator=(const Logs&) = delete;
	
	static std::mutex mtx;
	
public:
	void WriteMessage(LOG_LEVEL level, const std::string& message);
	void WriteToConsole(LOG_LEVEL level, const std::string& message);
	void WriteToFile(LOG_LEVEL level, const std::string& message);
	static Logs* Instance();
private:
	std::string GetTimeStamp();
	std::ofstream LogFile;
}

#define SLog Logs::Instance()

#define LOG_BODY(__LOG__LEVEL__, __MESSAGE__) \
	do { \
		SLog->Write(__LOG__LEVEL__, __MESSAGE__); \
		std::this_thread::sleep_for(std::chrono::seconds(1)); \
	} while(true);
	
#define LOG_TRACE(__MESSAGE__) \
	LOG_BODY(LOG_LEVEL::LOG_LEVEL_TRACE, __MESSAGE__)
	
#define LOG_DEBUG(__MESSAGE__) \
	LOG_BODY(LOG_LEVEL::LOG_LEVEL_DEBUG, __MESSAGE__)
	
#define LOG_INFO(__MESSAGE__) \
	LOG_BODY(LOG_LEVEL::LOG_LEVEL_INFO, __MESSAGE__)

#define LOG_WARNING(__MESSAGE__) \
	LOG_BODY(LOG_LEVEL::LOG_LEVEL_WARNING, __MESSAGE__)
	
#define LOG_ERROR(__MESSAGE__) \
	LOG_BODY(LOG_LEVEL::LOG_LEVEL_ERROR, __MESSAGE__)
	
#define LOG_FATAL(__MESSAGE__) \
	LOG_BODY(LOG_LEVEL::LOG_LEVEL_FATAL, __MESSAGE__)

#endif
