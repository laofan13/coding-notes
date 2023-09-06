#include <cstdio>
#include <string>
#include <chrono>
#include <ctime>

enum class LogsLevel { DEBUG, INFO, WARNING, ERROR };

class Logger { 
public:
    Logger & Instance() {
        static Logger logger;
        return logger;
    }

    void log(LogsLevel level, const char* format, std::va_list arguments) {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm *now_tm = std::localtime(&t);

        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now_tm);
    }

private:
    Logger() = default;
};

#define LOG_DEBUG(str, ...) {                                  \
    fprintf(stdout, "%s:%d. [DEBUG]: ", __FILE__, __LINE__); \
    fprintf(stdout, str, ##__VA_ARGS__);                     \
    fprintf(stdout, "\n");                                   \
}

#define LOG_INFO(str, ...) {                                 \
    fprintf(stdout, "%s:%d. [INFO]: ", __FILE__, __LINE__);  \
    fprintf(stdout, str, ##__VA_ARGS__);                     \
    fprintf(stdout, "\n");                                   \
}

#define LOG_WARNING(str, ...) {                                  \
    fprintf(stdout, "%s:%d. [WARNING]: ", __FILE__, __LINE__); \
    fprintf(stdout, str, ##__VA_ARGS__);                     \
    fprintf(stdout, "\n");                                   \
}

#define LOG_ERROR(str, ...) {                                 \
    fprintf(stderr, "%s:%d. [ERROR]: ", __FILE__, __LINE__);  \
    fprintf(stderr, str, ##__VA_ARGS__);                     \
    fprintf(stderr, "\n");                                   \
}