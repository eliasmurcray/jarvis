#ifndef OPENAI_CLIENT_H
#define OPENAI_CLIENT_H

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>
#include <filesystem>

using std::string;

const string OPENAI_COMPLETION_URL = "https://api.openai.com/v1/chat/completions";
const string OPENAI_TRANSCRIPTION_URL = "https://api.openai.com/v1/audio/transcriptions";

size_t write_cb(char *contents, size_t size, size_t nmemb, void *data);

class OpenAIClient {
public:
	OpenAIClient(const string& key);
	~OpenAIClient();
	string getCompletion(const string& prompt, const string& modelName = "gpt-3.5-turbo");
	string getTranscription(const string& audioFilePath, const string& modelName = "whisper-1");
private:
	string apiKey;
	CURL* curlSession;
};

#endif
