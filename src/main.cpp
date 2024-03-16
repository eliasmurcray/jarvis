#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using std::string;
using nlohmann::json;

string get_env(string const& varname);
string get_gpt_completion(const string& prompt, const string& key, const string& model = "gpt-3.5-turbo");
size_t write_cb(char *ptr, size_t size, size_t nmemb, void *data);

int main() {
	string key = get_env("GPT_KEY");
	if (key.empty()) {
		std::cout << "Error: No key found in $GPT_KEY\n";
		return EXIT_FAILURE;
	}

	string prompt;
	std::getline(std::cin, prompt);
	string res = get_gpt_completion(prompt, key);
	std::cout << res << "\n";

	return EXIT_SUCCESS;
}

string get_gpt_completion(const string& prompt, const string& key, const string& model) {
	const string base_url = "https://api.openai.com/v1/chat/completions";
	string res;
	CURL *c = curl_easy_init();
	if (c) {
		json req;
		req["model"] = model;
		req["messages"][0]["role"] = "user";
		req["messages"][0]["content"] = prompt;
		req["temperature"] = 0;

		string req_str = req.dump().c_str();

		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, ("Authorization: Bearer " + key).c_str());

		curl_easy_setopt(c, CURLOPT_URL, base_url.c_str());
		curl_easy_setopt(c, CURLOPT_POSTFIELDS, req_str.c_str());
		curl_easy_setopt(c, CURLOPT_POSTFIELDSIZE, req_str.length());
		curl_easy_setopt(c, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, write_cb);
		curl_easy_setopt(c, CURLOPT_WRITEDATA, &res);
		CURLcode code = curl_easy_perform(c);

		if (code != CURLE_OK) {
			std::cerr << "Curl request failed: " << curl_easy_strerror(code) << "\n";
		}
		curl_slist_free_all(headers);
	}
	curl_easy_cleanup(c);
	
	json json_res = json::parse(res);

	return json_res["choices"][0]["message"]["content"].get<string>();
}

size_t write_cb(char *contents, size_t size, size_t nmemb, void *data) {
	size_t total_size = size * nmemb;
	((string *)data)->append(contents, total_size);
	return total_size;
}

string get_env(string const& varname) {
	const char *val = getenv(varname.c_str());
	return val == NULL ? std::string() : std::string(val);
}
