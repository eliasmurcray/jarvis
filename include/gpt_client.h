#ifndef GPT_CLIENT_H
#define GPT_CLIENT_H

#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using std::string;

const string GPT_COMPLETION_URL = "https://api.openai.com/v1/chat/completions";

size_t write_cb(char *contents, size_t size, size_t nmemb, void *data);

class GPTClient {
public:
	GPTClient(const string& key);
	~GPTClient();
	string getCompletion(const string& prompt, const string& modelName = "gpt-3.5-turbo");
private:
	string key;
	CURL* curlSession;
};

#endif
