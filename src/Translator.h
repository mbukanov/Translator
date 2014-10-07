#include <iostream>
#include <string>
#include <curl/curl.h>
#include <vector>

class Translator
{
public:
	typedef std::vector<std::pair<std::string /*key*/, std::string /*description*/> > Langs; 
	Translator(std::string &key);
	Translator(std::string key, std::string myLang);
	Translator();
	~Translator();

	void setApiKey(std::string &key);
	std::string getApiKey() const;
	Langs getLangs() const;
	std::string getMyLang() const;
	std::string translate(std::string from, std::string to, std::string text);
	std::string translate(std::string to, std::string text);
	std::string detect(std::string text);


private:
	std::string _apiKey;
	std::string _myLang;

	CURL* curl;
	std::string _curlBuffer;
	char _curlErrorBuffer[CURL_ERROR_SIZE];

	static int writer(char *data, size_t size, size_t nmemb, std::string *buffer);

};