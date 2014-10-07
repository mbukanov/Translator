#include <iostream>
#include <getopt.h>
#include <iomanip>
#include <fstream>
#include <string>
#include "Translator.h"


/*
todo:
	translator [-i fromlang] [-f filename | text] -o tolang [-o outputfilename]

*/

void printHelp()
{
	std::cout<<"Help:"<<std::endl
		<<"translator [-i [ru, en, ...]] -o {ru,en, ...} -t TEXT | -f FILENAME"<<std::endl
		<<"-i"<<std::setw(50)<<"input language, else auto detect by 'text'"<<std::endl
		<<"-o"<<std::setw(50)<<"output language"<<std::endl
		<<"-t"<<std::setw(50)<<"input text in '-i <lang>' language"<<std::endl
		<<"-f"<<std::setw(50)<<"text from filename"<<std::endl;

	std::cout<<"Example: translator -o ru -t Hello"<<std::endl;
	std::cout<<"Example: translator -i ru -o en -t Привет"<<std::endl;
}

bool getParams(	int argc, 
				char** argv, 
				std::string &from, 
				std::string &to,
				std::string &filename,
				std::string &text )
{
	int rez = 0;
	if(argc == 1)
	{
		printHelp();
		return 0;
	}
	while( (rez = getopt(argc, argv, "i:o:f:t:h")) != -1)
	{
		switch(rez)
		{
			case 'i':
				from = optarg;
				break;
			case 'o':
				to = optarg;
				break;
			case 'f':
				filename = optarg;
				break;
			case 't':
				text = optarg;
				break;
			case 'h':
				printHelp();
				return false;
			default:
				printHelp();
				return false;
		}
	}

	if(to.empty())
	{
		printHelp();
		return false;
	}
	if(text.empty() && filename.empty())
	{
		printHelp();
		return false;
	}else if(!filename.empty())
	{
		// todo: file streams 

		std::string line = "";
		std::ifstream myfile (filename.c_str());
		if (myfile.is_open())
  		{
  			text = "";
    		while ( std::getline (myfile,line) )
    		{
      			text += line;
    		}
    		myfile.close();
  		}else{
  			std::cout<<"Error: file "<<filename<<" is fail\n";
  			return false;
  		}
	}

	return true;
}


int main(int argc, char** argv)
{
	std::string from = "";
	std::string to = "";
	std::string filename = "";
	std::string text = "";
	
	if(!getParams(argc, argv, from, to, filename, text))
		return 0;

	std::string key = "trnsl.1.1.20141007T170000Z.ab9f125e2cb8fd25.cce2799f5b348a3e6e918f5e37f2d4d3f5a11559";
	Translator tr(key);
	

	if(from.empty())
	{
		std::cout<<tr.translate(to, text)<<std::endl;
	}else{
		std::cout<<tr.translate(from, to, text)<<std::endl;
	}

	return 0;
}