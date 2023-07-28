#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <vector>

enum class LogLevel { DEBUG,
                      INFO,
                      WARNING,
                      ERROR };

class Logger {
public:
    virtual void log(LogLevel level, const std::string &message) = 0;
};

class ConsoleLogger : public Logger {
public:
    void log(LogLevel level, const std::string &message) override {
        std::string level_str;
        switch (level) {
        case LogLevel::DEBUG:
            level_str = "DEBUG";
            break;
        case LogLevel::INFO:
            level_str = "INFO";
            break;
        case LogLevel::WARNING:
            level_str = "WARNING";
            break;
        case LogLevel::ERROR:
            level_str = "ERROR";
            break;
        }
        std::cout << "[" << level_str << "] " << message << std::endl;
    }
};

class FileLogger : public Logger {
public:
    FileLogger(const std::string &filename, std::size_t max_size) :
        filename_(filename), max_size_(max_size), current_size_(0) {
        std::ofstream ofs(filename, std::ios::app);
        ofs.close();
        current_size_ = std::filesystem::file_size(filename);
    }

    void log(LogLevel level, const std::string &message) override {
        std::string level_str;
        switch (level) {
        case LogLevel::DEBUG:
            level_str = "DEBUG";
            break;
        case LogLevel::INFO:
            level_str = "INFO";
            break;
        case LogLevel::WARNING:
            level_str = "WARNING";
            break;
        case LogLevel::ERROR:
            level_str = "ERROR";
            break;
        }

        std::ofstream ofs(filename_, std::ios::app);
        ofs << "[" << level_str << "] " << message << std::endl;
        ofs.close();
        current_size_ = std::filesystem::file_size(filename_);

        if (current_size_ > max_size_) {
            roll();
        }
    }

private:
    void roll() {
        std::string filename = filename_ + "." + std::to_string(roll_count_++);
        std::filesystem::rename(filename_, filename);
        std::ofstream ofs(filename_, std::ios::app);
        ofs.close();
        current_size_ = std::filesystem::file_size(filename_);
    }

    std::string filename_;
    std::size_t max_size_;
    std::size_t current_size_;
    std::size_t roll_count_ = 0;
};

// 定义日志类
class Log {
public:
    Log(LogLevel level, const std::string &message) :
        level_(level), message_(message) {
        std::time_t now = std::time(nullptr);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");
        timestamp_ = ss.str();
    }

    static void set_logger(Logger *logger) {
        logger_ = logger;
    }

    static void set_file_logger(const std::string &filename,
                                std::size_t max_size) {
        logger_ = new FileLogger(filename, max_size);
    }

    static void set_console_logger() {
        logger_ = new ConsoleLogger();
    }

    static void debug(const std::string &message) {
        log(LogLevel::DEBUG, message);
    }

    static void info(const std::string &message) {
        log(LogLevel::INFO, message);
    }

    static void warning(const std::string &message) {
        log(LogLevel::WARNING, message);
    }

    static void error(const std::string &message) {
        log(LogLevel::ERROR, message);
    }

private:
    static void log(LogLevel level, const std::string &message) {
        if (logger_ != nullptr) {
            Log log(level, message);
            logger_->log(level, log.to_string());
        }
    }

    std::string to_string() const {
        std::stringstream ss;
        ss << timestamp_ << " " << message_;
        return ss.str();
    }

    static Logger *logger_;
    LogLevel level_;
    std::string timestamp_;
    std::string message_;
};

Logger *Log::logger_ = nullptr;