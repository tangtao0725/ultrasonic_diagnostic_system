#ifndef PUBLIC_H
#define PUBLIC_H

#include <string>

#ifdef X86
#define printX86(fmt,args...)       printf(fmt,## args)
#else
#define printX86(fmt,args...)
#endif

std::string trim(std::string &str, char ch = ' ');

#define VERSION		"0.03"
#define FILE_VERSION	"0.03"

#endif
