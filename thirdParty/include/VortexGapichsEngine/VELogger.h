#ifndef VORTEX_ENGINE_LOGGER
#define VORTEX_ENGINE_LOGGER

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <mutex>
#include <ctime>
#include <sstream>
#include "imgui/imgui.h"

#define SEND_MSG(type, sender, message) VELogger::log(type, sender, message, __FILE__, __LINE__)
#define SEND_MSG_TERMINATE(type, sender, message) VELogger::log(type, sender, message, __FILE__, __LINE__); exit(0)

enum LogLevel {
	ERROR,
	INFO,
	WARN,
};

class VELogger
{
private:
	static std::ofstream logFile;
	static std::mutex logMutex;
	static std::vector<std::pair<LogLevel, std::string>> logBuffer;

	static std::string logLevelToString(LogLevel level);
	ImVec4 getColorForLogLevel(LogLevel level);
	static std::string getCurrentTimestamp();
public:

	VELogger(){}

	void setLogFile(const std::string& filename) {
		logFile = std::ofstream(filename, std::ios::app);
		if (!logFile.is_open()) {
			throw std::runtime_error("Unable to open log file");
		}
	}

	~VELogger() {
		if (logFile.is_open()) {
			logFile.close();
		}
	}

	static void log(LogLevel level, std::string sender, std::string message, const char* file, int line);
	void renderImGui();
};

#endif // !VORTEX_ENGINE_LOGGER
