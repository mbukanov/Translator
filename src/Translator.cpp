#include "Translator.h"

Translator::Translator(std::string &key) : _apiKey(key)
{
	curl = curl_easy_init();
	if(!curl)
	{
		// throw
	}

	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
    curl_easy_setopt(curl, CURLOPT_PORT, 443);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &Translator::writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &_curlBuffer);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, _curlErrorBuffer);

}

Translator::Translator(std::string key, std::string myLang) : _apiKey(key), _myLang(myLang)
{
	curl = curl_easy_init();
}

Translator::~Translator()
{
}

Translator::Langs Translator::getLangs() const
{
	Translator::Langs langs;
	std::string url = "https://translate.yandex.net/api/v1.5/tr.json/getLangs?key=";
	url += getApiKey();
	url += "&ui=";
	url += getMyLang();

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());;
	CURLcode res = curl_easy_perform(curl);
	if(res != CURLE_OK)
	{
		// error
	}

	// parse buffer;

	return langs;
}

int Translator::writer(char *data, size_t size, size_t nmemb, std::string *buffer)
{
    int result = 0;
    if(buffer != NULL)
    {
        buffer->append(data, size * nmemb);
        result = size * nmemb;
    }
    return result;
}

std::string Translator::getMyLang() const
{
	if(!_myLang.empty())
		return _myLang;

	// curl detect
}

void Translator::setApiKey(std::string &key)
{
	_apiKey = key;
}

std::string Translator::getApiKey() const
{
	return _apiKey;
}

std::string Translator::translate(std::string from, std::string to, std::string text)
{
	CURLcode res;

	std::string request = "https://translate.yandex.net/api/v1.5/tr.json/translate?key=";
	request += getApiKey();
	request += "&lang=";
	request += from+"-"+to;
	request += "&text="+text;

	curl_easy_setopt(curl, CURLOPT_URL, request.c_str());

    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
    {
    	// fix this
    	curl_easy_cleanup(curl);
    	return "";
    }

    return _curlBuffer;
}

std::string Translator::translate(std::string to, std::string text)
{
	std::string from = getMyLang();

	return translate(from, to, text);
}
