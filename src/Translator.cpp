#include "Translator.h"

Translator::Translator(std::string &key) : _apiKey(key), _myLang("")
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
	if(curl)
	{
		curl_easy_cleanup(curl);
	}
}

Translator::Langs Translator::getLangs() const
{
	_curlBuffer = "";
	Translator::Langs langs;
	std::string mylang = getMyLang();
	if(mylang.empty())
		mylang = "en";	// default

	std::string url = "https://translate.yandex.net/api/v1.5/tr.json/getLangs?key=";
	url += getApiKey();
	url += "&ui=";
	url += mylang;

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());;
	CURLcode res = curl_easy_perform(curl);
	if(res != CURLE_OK)
	{
		// error
	}


	Json::Reader reader;
	Json::Value root;
	reader.parse(_curlBuffer, root);

	for(Json::ValueIterator it = root["langs"].begin(); it != root["langs"].end(); it++)
	{
		langs.push_back(std::make_pair(	
										it.key().asString(),  	// key
										(*it).asString()		// value
									));
	}

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

	return "";
}

std::string Translator::detect(std::string text)
{
	std::string myLang = "";
	_curlBuffer = "";
	std::string request = "https://translate.yandex.net/api/v1.5/tr.json/detect?key=";
	request += getApiKey();
	request += "&text=";
	request += text;

	curl_easy_setopt(curl, CURLOPT_URL, request.c_str()); 

	CURLcode res = curl_easy_perform(curl);
	if(res != CURLE_OK)
	{
		// error
		return "";
	}

	Json::Reader reader;
	Json::Value root;
	reader.parse(_curlBuffer, root);
	myLang = root["lang"].asString();

	return myLang;
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
	std::string result = "";
	CURLcode res;
	_curlBuffer = "";

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

    Json::Reader reader;
    Json::Value root;
    reader.parse(_curlBuffer, root);

    for(int i = 0; i < root["text"].size(); i++)
    	result += root["text"][i].asString()+" ";


    return result;
}

std::string Translator::translate(std::string to, std::string text)
{
	std::string from = getMyLang();
	if(from.empty())
		from = detect(text);

	return translate(from, to, text);
}
