#ifndef LOGGER_CPP_
#define LOGGER_CPP_

#include <iostream>
#include "logger.h"
using namespace std;

logger *logger::instance = nullptr;

void logger::log_error(const string s) {
	cout << "ERROR: " << s << endl;
}

void logger::log_info(const string s) {
	cout << "INFO: " << s << endl;
}

logger* logger::getInstance() {
	if (!instance)
		instance = new logger();
	return instance;
}

#endif /* LOGGER_CPP_ */
