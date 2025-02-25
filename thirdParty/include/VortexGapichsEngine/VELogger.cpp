#include "VELogger.h"

std::mutex VELogger::logMutex;
std::vector<std::pair<LogLevel, std::string>> VELogger::logBuffer;
std::ofstream VELogger::logFile;

std::string VELogger::logLevelToString(LogLevel level)
{
    switch (level) {
    case LogLevel::INFO: return "INFO";
    case LogLevel::WARN: return "WARNING";
    case LogLevel::ERROR: return "ERROR";
    default: return "UNKNOWN";
    }
}

ImVec4 VELogger::getColorForLogLevel(LogLevel level)
{
    switch (level) {
    case LogLevel::INFO: return ImVec4(0.0f, 1.0f, 0.0f, 1.0f); 
    case LogLevel::WARN: return ImVec4(1.0f, 1.0f, 0.0f, 1.0f); 
    case LogLevel::ERROR: return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    default: return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);             
    }
}

std::string VELogger::getCurrentTimestamp()
{
    std::time_t now = std::time(nullptr);
    std::tm localTime;
    localtime_s(&localTime, &now);
    std::ostringstream oss;
    oss << (localTime.tm_year + 1900) << "-"
        << (localTime.tm_mon + 1) << "-"
        << localTime.tm_mday << " "
        << localTime.tm_hour << ":"
        << localTime.tm_min << ":"
        << localTime.tm_sec;
    return oss.str();
}

void VELogger::log(LogLevel level, std::string sender, std::string message, const char* file, int line)
{
    std::lock_guard<std::mutex> guard(logMutex);
    std::string logLevelStr = logLevelToString(level);
    std::string timestamp = getCurrentTimestamp();
	std::string Errorline = std::to_string(line);
    std::string logMessage;
    switch (level)
    {
    case ERROR:
        logMessage = "[" + timestamp + "] [" + logLevelStr + "] " + "[" + sender + "]" + message + " file: " + file + " at: " + Errorline;
        break;
    case INFO:
        logMessage = "[" + timestamp + "] [" + logLevelStr + "] " + message;
        break;
    case WARN:
        logMessage = "[" + timestamp + "] [" + logLevelStr + "] " + "[" + sender + "]" + message;
        break;
    default:
        logMessage = "Incorrect Log Level";
        break;
    }

    logFile << logMessage << std::endl;
    logBuffer.push_back({ level, logMessage });
}

void VELogger::renderImGui()
{
    ImGui::Begin("Logger");
    for (const auto& message : logBuffer) {
        ImVec4 color = getColorForLogLevel(message.first);
        ImGui::TextColored(color, "%s", message.second.c_str());
    }
    ImGui::End();
}
