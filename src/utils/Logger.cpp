#include "Logger.hpp"

// Initialize static member
Logger* Logger::instance = nullptr;

Logger::Logger() 
    : currentLevel(Level::INFO), consoleOutput(true), logFilename("hotswap_system.log") {
    
    // Open log file
    logFile.open(logFilename, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file: " << logFilename << std::endl;
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

Logger& Logger::getInstance() {
    if (!instance) {
        instance = new Logger();
    }
    return *instance;
}

void Logger::log(Level level, const std::string& message, const std::string& module) {
    // Skip if log level is too low
    if (level < currentLevel) {
        return;
    }

    std::lock_guard<std::mutex> lock(logMutex);
    
    std::string timestamp = getCurrentTimestamp();
    std::string levelStr = levelToString(level);
    std::string logEntry;

    // Format log entry
    if (module.empty()) {
        logEntry = "[" + timestamp + "] [" + levelStr + "] " + message;
    } else {
        logEntry = "[" + timestamp + "] [" + levelStr + "] [" + module + "] " + message;
    }

    // Write to file
    if (logFile.is_open()) {
        logFile << logEntry << std::endl;
        logFile.flush();
    }

    // Write to console with colors
    if (consoleOutput) {
        std::string colorCode = getColorCode(level);
        std::string resetColor = getResetColor();
        std::cout << colorCode << logEntry << resetColor << std::endl;
    }
}

std::string Logger::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count();
    
    return ss.str();
}

std::string Logger::levelToString(Level level) {
    switch (level) {
        case Level::DEBUG: return "DEBUG";
        case Level::INFO: return "INFO";
        case Level::WARNING: return "WARNING";
        case Level::ERROR: return "ERROR";
        case Level::CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

std::string Logger::getColorCode(Level level) {
    switch (level) {
        case Level::DEBUG: return "\033[36m"; // Cyan
        case Level::INFO: return "\033[32m";  // Green
        case Level::WARNING: return "\033[33m"; // Yellow
        case Level::ERROR: return "\033[31m"; // Red
        case Level::CRITICAL: return "\033[35m"; // Magenta
        default: return "\033[0m"; // Reset
    }
}

std::string Logger::getResetColor() {
    return "\033[0m";
}

// Public logging methods
void Logger::debug(const std::string& message, const std::string& module) {
    log(Level::DEBUG, message, module);
}

void Logger::info(const std::string& message, const std::string& module) {
    log(Level::INFO, message, module);
}

void Logger::warning(const std::string& message, const std::string& module) {
    log(Level::WARNING, message, module);
}

void Logger::error(const std::string& message, const std::string& module) {
    log(Level::ERROR, message, module);
}

void Logger::critical(const std::string& message, const std::string& module) {
    log(Level::CRITICAL, message, module);
}

// Configuration methods
void Logger::setLogLevel(Level level) {
    std::lock_guard<std::mutex> lock(logMutex);
    currentLevel = level;
}

void Logger::setLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(logMutex);
    
    if (logFile.is_open()) {
        logFile.close();
    }
    
    logFilename = filename;
    logFile.open(logFilename, std::ios::app);
    
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file: " << logFilename << std::endl;
    }
}

void Logger::enableConsoleOutput(bool enable) {
    std::lock_guard<std::mutex> lock(logMutex);
    consoleOutput = enable;
}