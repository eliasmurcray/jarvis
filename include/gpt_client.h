#ifndef GPT_CLIENT_H
#define GPT_CLIENT_H

#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

const std::string GPT_COMPLETION_URL = "https://api.openai.com/v1/chat/completions";

size_t write_cb(char *contents, size_t size, size_t nmemb, void *data);

class GPTClient {
public:
	GPTClient(const std::string& key);
	~GPTClient();
	std::string getCompletion(const std::string& prompt, const std::string& modelName = "gpt-3.5-turbo");
private:
	std::string key;
	CURL* curlSession;
};

#endif
