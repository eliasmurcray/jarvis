#include "gpt_client.h"
#include "jarvis_utils.h"
#include <iostream>
#include <string>

using std::string;

int main() {
	string key = getEnvironmentVariable("GPT_KEY");
	if (key.empty()) {
		std::cerr << "Error: Environment variable 'GPT_KEY' not found\n";
		return EXIT_FAILURE;
	}

	GPTClient client(key);
	
	string prompt;
	std::getline(std::cin, prompt);
	string res = client.getCompletion(prompt);
	std::cout << res << "\n";

	return EXIT_SUCCESS;
}
