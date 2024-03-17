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
	string key = getEnvironmentVariable("OPENAI_API_KEY");
	if (key.empty()) {
		cerr << "Error: Environment variable 'OPENAI_API_KEY' not found\n";
		return EXIT_FAILURE;
	}

	GPTClient client(key);
	
	cout << "Jarvis initialized. Type 'exit' to terminate the session.\n\n";
	
	string prompt;
	while (1) {
		cout << "# ";
		getline(cin, prompt);
		if (prompt == "exit") break;
		string res = client.getCompletion(prompt);
		cout << "\n" << res << "\n\n";
	}

	cout << "Session terminated.\n";

	return EXIT_SUCCESS;
}
