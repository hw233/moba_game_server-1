#ifndef __LOGGER_H__
#define __LOGGER_H__

enum {
	DEBUG = 0,
	WARNING=1,
	ERRORR=2,
};

#define log_debug(msg, ...) logger::log(__FILE__, __LINE__, DEBUG, msg, ## __VA_ARGS__);
#define log_warning(msg, ...) logger::log(__FILE__, __LINE__, WARNING, msg, ## __VA_ARGS__);
#define log_error(msg, ...) logger::log(__FILE__, __LINE__, ERRORR, msg, ## __VA_ARGS__);

class logger {
public:
	static void init(char* path, char* prefix, bool std_output = false);
	static void log(const char* file_name, 
	                int line_num, 
	                int level, const char* msg, ...);
};


#endif

