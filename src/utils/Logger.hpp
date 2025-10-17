#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <ctime>
#include <iomanip>
#include <iostream>

class Logger {
public:
    // Log levels
    enum class Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };

    // Singleton instance access
    static Logger& getInstance();

    // Logging methods
    void debug(const std::string& message, const std::string& module = "");
    void info(const std::string& message, const std::string& module = "");
    void warning(const std::string& message, const std::string& module = "");
    void error(const std::string& message, const std::string& module = "");
    void critical(const std::string& message, const std::string& module = "");

    // Configuration
    void setLogLevel(Level level);
    void setLogFile(const std::string& filename);
    void enableConsoleOutput(bool enable);

    // Prevent copying
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    Logger();
    ~Logger();

    void log(Level level, const std::string& message, const std::string& module);
    std::string levelToString(Level level);
    std::string getCurrentTimestamp();
    std::string getColorCode(Level level);
    std::string getResetColor();

    // Member variables
    static Logger* instance;
    std::ofstream logFile;
    std::mutex logMutex;
    Level currentLevel;
    bool consoleOutput;
    std::string logFilename;
};