#include "jarvis_utils.h"

using std::string;

string getEnvironmentVariable(const string& name) {
	const char *value = getenv(name.c_str());
	return value == NULL ? string() : string(value);
}
