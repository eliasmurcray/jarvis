#include "openai_client.h"

using std::string;
using std::runtime_error;
using std::filesystem::exists;
using nlohmann::json;

size_t writeCallback(char *contents, size_t size, size_t nmemb, void *data) {
	size_t total_size = size * nmemb;
	((string *)data)->append(contents, total_size);
	return total_size;
}

OpenAIClient::OpenAIClient(const string& apiKey): apiKey(apiKey), curlSession(curl_easy_init()) {
	if (!curlSession) {
		throw runtime_error("Failed to initialize CURL session.\n");
	}
}

OpenAIClient::~OpenAIClient() {
	if (curlSession) {
		curl_easy_cleanup(curlSession);
		curlSession = nullptr;
	}
}

string OpenAIClient::getCompletion(const string& prompt, const string& modelName) {
	string res;
	json req;
	req["model"] = modelName;
	req["messages"][0]["role"] = "user";
	req["messages"][0]["content"] = prompt;
	req["temperature"] = 0;
	
	string req_str = req.dump().c_str();

	struct curl_slist *headers = curl_slist_append(NULL, "Content-Type: application/json");
	headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());

	curl_easy_setopt(curlSession, CURLOPT_URL, OPENAI_COMPLETION_URL.c_str());
	curl_easy_setopt(curlSession, CURLOPT_POSTFIELDS, req_str.c_str());
	curl_easy_setopt(curlSession, CURLOPT_POSTFIELDSIZE, req_str.length());
	curl_easy_setopt(curlSession, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curlSession, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(curlSession, CURLOPT_WRITEDATA, &res);
	CURLcode code = curl_easy_perform(curlSession);

	if (code != CURLE_OK) {
		throw runtime_error("Curl request failed: " + string(curl_easy_strerror(code)));
	}

	curl_slist_free_all(headers);

	json json_res = json::parse(res);
	if (!json_res["error"].is_null()) {
		throw runtime_error(json_res["error"].dump());
	}
	return json_res["choices"][0]["message"]["content"].get<string>();
}

string OpenAIClient::getTranscription(const string& audioFilePath, const string& modelName) {
	if (!exists(audioFilePath)) {
		throw runtime_error("File in path '" + audioFilePath + "' does not exist");
	}

	string res;

	curl_mime *mime = curl_mime_init(curlSession);
	curl_mimepart *part = curl_mime_addpart(mime);
	curl_mime_name(part, "file");
	curl_mime_filedata(part, audioFilePath.c_str());

	part = curl_mime_addpart(mime);
	curl_mime_name(part, "model");
	curl_mime_data(part, modelName.c_str(), CURL_ZERO_TERMINATED);

	part = curl_mime_addpart(mime);
	curl_mime_name(part, "response_format");
	curl_mime_data(part, "text", CURL_ZERO_TERMINATED);

	struct curl_slist *headers = curl_slist_append(NULL, ("Authorization: Bearer " + apiKey).c_str());

	curl_easy_setopt(curlSession, CURLOPT_URL, OPENAI_TRANSCRIPTION_URL.c_str());
	curl_easy_setopt(curlSession, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curlSession, CURLOPT_MIMEPOST, mime);
	curl_easy_setopt(curlSession, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(curlSession, CURLOPT_WRITEDATA, &res);

	CURLcode code = curl_easy_perform(curlSession);

	if (code != CURLE_OK) {
		throw runtime_error("Curl request failed: " + string(curl_easy_strerror(code)));
	}
	
	curl_slist_free_all(headers);

	return res;
}
