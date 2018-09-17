#ifndef FILEOP_FILE_PARSER_H
#define FILEOP_FILE_PARSER_H

#include <fstream>

class FileParser
{
private:
	ifstream file;
public:
	FileParser(const char *filename);
	~FileParser();
};

#endif // FILEOP_FILE_PARSER_H
