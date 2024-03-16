#include "gpt_client.h"
#include "jarvis_utils.h"
#include <iostream>
#include <string>

using std::string;
using std::cerr;
using std::getline;
using std::cout;
using std::cin;

int main() {
	string key = getEnvironmentVariable("GPT_KEY");
	if (key.empty()) {
		cerr << "Error: Environment variable 'GPT_KEY' not found\n";
		return EXIT_FAILURE;
	}

	GPTClient client(key);
	
	string prompt;
	getline(cin, prompt);
	string res = client.getCompletion(prompt);
	cout << res << "\n";

	return EXIT_SUCCESS;
}
