#include "jarvis_utils.h"

std::string getEnvironmentVariable(const std::string& name) {
	const char *value = getenv(name.c_str());
	return value == NULL ? std::string() : std::string(value);
}
