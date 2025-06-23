#include "logger.h"

const std::string Logger::LOG_FILE = "app.log";

Logger::Logger() : logLevel(3), fileLoggingEnabled(true), consoleOutputEnabled(true), logCount(0) {
    logFileStream.open(LOG_FILE, std::ios::app);
    if (!logFileStream.is_open()) {
        std::cerr << "Error opening log file: " << LOG_FILE << std::endl; // ошибка открытия файла логов
    }
}

Logger::~Logger() {
    if (logFileStream.is_open()) {
        logFileStream.close();
    }
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex);
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm timeinfo;

#ifdef _WIN32
    localtime_s(&timeinfo, &time);
#else
    localtime_r(&time, &timeinfo);
#endif

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    std::string timestamp(buffer);
    std::string formattedMessage = "[" + timestamp + "] " + message;

    if (fileLoggingEnabled && logFileStream.is_open()) {
        logFileStream << formattedMessage << std::endl;
    }

    if (consoleOutputEnabled) {
        std::cout << formattedMessage << std::endl;
    }

    logCount++;
}

void Logger::setLogLevel(int level) {
    std::lock_guard<std::mutex> lock(mutex);
    logLevel = level;
}

void Logger::enableFileLogging(bool enable) {
    std::lock_guard<std::mutex> lock(mutex);
    fileLoggingEnabled = enable;
}

void Logger::enableConsoleOutput(bool enable) {
    std::lock_guard<std::mutex> lock(mutex);
    consoleOutputEnabled = enable;
}

void Logger::clearLogs() {
    std::lock_guard<std::mutex> lock(mutex);
    logFileStream.close();
    logFileStream.open(LOG_FILE, std::ofstream::trunc);
    if (!logFileStream.is_open()) {
        std::cerr << "Error opening log file for clearing: " << LOG_FILE << std::endl; // ошибка открытия файла логов для очистки
    }
}

int Logger::getLogCount() {
    std::lock_guard<std::mutex> lock(mutex);
    return logCount;
}

void Logger::flush() {
    std::lock_guard<std::mutex> lock(mutex);
    if (logFileStream.is_open()) {
        logFileStream.flush();
    }
}
