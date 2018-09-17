#include "fileParser.h"

FileParser::FileParser(const char *filename)
	: file(filename)
{
}

FileParser::~FileParser() 
{
	file.close();
}