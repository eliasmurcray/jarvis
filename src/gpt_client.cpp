#include "gpt_client.h"
#include <iostream>

using std::string;
using nlohmann::json;

size_t write_cb(char *contents, size_t size, size_t nmemb, void *data) {
	size_t total_size = size * nmemb;
	((string *)data)->append(contents, total_size);
	return total_size;
}

GPTClient::GPTClient(const string& key): curlSession(curl_easy_init()) {
	if (!curlSession) {
		throw std::runtime_error("Error: Failed to initialize CURL session.\n");
	}
	this->key = key;
}

GPTClient::~GPTClient() {
	if (curlSession) {
		curl_easy_cleanup(curlSession);
		curlSession = nullptr;
	}
}

std::string GPTClient::getCompletion(const string& prompt, const string& model) {
	std::string res;
	json req;
	req["model"] = model;
	req["messages"][0]["role"] = "user";
	req["messages"][0]["content"] = prompt;
	req["temperature"] = 0;
	
	std::string req_str = req.dump().c_str();

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, ("Authorization: Bearer " + key).c_str());

	curl_easy_setopt(curlSession, CURLOPT_URL, GPT_COMPLETION_URL.c_str());
	curl_easy_setopt(curlSession, CURLOPT_POSTFIELDS, req_str.c_str());
	curl_easy_setopt(curlSession, CURLOPT_POSTFIELDSIZE, req_str.length());
	curl_easy_setopt(curlSession, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curlSession, CURLOPT_WRITEFUNCTION, write_cb);
	curl_easy_setopt(curlSession, CURLOPT_WRITEDATA, &res);
	CURLcode code = curl_easy_perform(curlSession);

	if (code != CURLE_OK) {
		throw std::runtime_error("Curl request failed: " + std::string(curl_easy_strerror(code)));
	}

	curl_slist_free_all(headers);

	json json_res = json::parse(res);
	if (!json_res["error"].is_null()) {
		throw std::runtime_error("Error: " + json_res["error"].dump());
	}
	return json_res["choices"][0]["message"]["content"].get<string>();
}
