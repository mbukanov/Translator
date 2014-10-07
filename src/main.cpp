#include <iostream>
#include "Translator.h"


/*
todo:
	translator fromlang [-f filename | text] tolang [-o outputfilename]

*/


int main(int argc, char** argv)
{
	std::string key = "yourapi";
	Translator tr(key);
	std::string text = tr.translate("ru", "en", "Привет");

	std::cout<<"Text: "<<text<<std::endl;



	return 0;
}