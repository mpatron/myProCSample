//logger.h
#define LOG_PRINT(...) log_print(__FILE__, __LINE__, __VA_ARGS__ )
void log_print(char* filename, int line, char *fmt,...);