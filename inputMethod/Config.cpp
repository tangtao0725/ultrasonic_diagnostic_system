#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include "Config.h"
#include "public.h"

const int MAXLEN = 256;

Config::Config(const char* config_file)
{
	std::ifstream ifs(config_file);
	std::string line;
	char key[MAXLEN],value[MAXLEN];
	std::string  sKey,sValue;
		
	while(ifs)
	{
		ifs>>line;
		if(ifs.eof()) break;

		int sPos = line.find_first_of('=');
		if(sPos != -1){
			line.replace(sPos,1," ");
		}

		key[0]=value[0]=0;
		sscanf(line.c_str(),"%s %s",key,value);
		sKey=key;
		sValue=value;
		trim(sKey);
		trim(sValue);
		trim(sValue,'\n');
		
		if(sKey.size()>0&&sValue.size()>0){
			if(sKey[0]!=';'){
				m_values[sKey]=sValue;
				printX86("%s=%s\n",sKey.c_str(),sValue.c_str());
			}
		}
	}
}

std::string Config::get_as_string(const char* key_name)
{
	std::string s(key_name);
	if(m_values.find(s)!=m_values.end()){
		return m_values[s];
	}
	else{
		s="";
		return s;
	}
}

int Config::get_as_int(const char* key_name)
{
	std::string s=get_as_string(key_name);
	if(s.size()>0){
		return atoi(s.c_str());
	}
	else{
		return 0;
	}
}

