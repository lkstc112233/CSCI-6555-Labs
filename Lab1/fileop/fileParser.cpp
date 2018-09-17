#include "fileParser.h"

FileParser::FileParser(const char *filename)
	: file(filename)
{
	if (file) {
		valid = true;
	}
}

FileParser::~FileParser() 
{
	file.close();
}