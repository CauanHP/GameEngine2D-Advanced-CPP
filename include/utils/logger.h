#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <memory>
#include <chrono>
#include <sstream>
#include <iostream>

namespace GameEngine2D {

enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3,
    CRITICAL = 4
};

class Logger {
public:
    static Logger& getInstance();
    
    void setLogLevel(LogLevel level);
    void setLogFile(const std::string& filename);
    void enableConsoleOutput(bool enable);
    
    template<typename... Args>
    void log(LogLevel level, const std::string& format, Args&&... args);
    
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void critical(const std::string& message);

private:
    Logger() = default;
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    std::string formatMessage(LogLevel level, const std::string& message);
    std::string getCurrentTime();
    std::string getLevelString(LogLevel level);
    
    LogLevel m_logLevel = LogLevel::INFO;
    std::unique_ptr<std::ofstream> m_logFile;
    bool m_consoleOutput = true;
    std::mutex m_mutex;
};

// Template implementation
template<typename... Args>
void Logger::log(LogLevel level, const std::string& format, Args&&... args) {
    if (level < m_logLevel) return;
    
    std::ostringstream oss;
    formatString(oss, format, std::forward<Args>(args)...);
    
    std::lock_guard<std::mutex> lock(m_mutex);
    std::string message = formatMessage(level, oss.str());
    
    if (m_consoleOutput) {
        std::cout << message << std::endl;
    }
    
    if (m_logFile && m_logFile->is_open()) {
        *m_logFile << message << std::endl;
        m_logFile->flush();
    }
}

// Helper function for string formatting
template<typename... Args>
void formatString(std::ostringstream& oss, const std::string& format, Args&&... args) {
    size_t pos = 0;
    size_t argIndex = 0;
    
    while (pos < format.length()) {
        size_t placeholder = format.find("{}", pos);
        if (placeholder == std::string::npos) {
            oss << format.substr(pos);
            break;
        }
        
        oss << format.substr(pos, placeholder - pos);
        
        if (argIndex < sizeof...(args)) {
            ((oss << args), ...);
            argIndex++;
        }
        
        pos = placeholder + 2;
    }
}

// Convenience macros
#define LOG_DEBUG(msg) Logger::getInstance().debug(msg)
#define LOG_INFO(msg) Logger::getInstance().info(msg)
#define LOG_WARNING(msg) Logger::getInstance().warning(msg)
#define LOG_ERROR(msg) Logger::getInstance().error(msg)
#define LOG_CRITICAL(msg) Logger::getInstance().critical(msg)

#define LOG_DEBUG_FMT(format, ...) Logger::getInstance().log(LogLevel::DEBUG, format, __VA_ARGS__)
#define LOG_INFO_FMT(format, ...) Logger::getInstance().log(LogLevel::INFO, format, __VA_ARGS__)
#define LOG_WARNING_FMT(format, ...) Logger::getInstance().log(LogLevel::WARNING, format, __VA_ARGS__)
#define LOG_ERROR_FMT(format, ...) Logger::getInstance().log(LogLevel::ERROR, format, __VA_ARGS__)
#define LOG_CRITICAL_FMT(format, ...) Logger::getInstance().log(LogLevel::CRITICAL, format, __VA_ARGS__)

} // namespace GameEngine2D
