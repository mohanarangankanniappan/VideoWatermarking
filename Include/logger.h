#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>

using namespace std;

class logger {
	static logger *instance;
	logger() {
	}
public:
	void log_error(string s);

	void log_info(string s);

	static logger* getInstance();
};

#endif /* LOGGER_H_ */
