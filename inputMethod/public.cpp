#include "public.h"

std::string trim(std::string &str, char ch)
{
	if(str.length() == 0){
		return str;
	}

	int sPos = str.find_first_not_of(ch);
	int ePos = str.find_last_not_of(ch);
	if(sPos == -1){
		return str.erase();
	}

	return str.substr(sPos, ePos - sPos + 1);
}

