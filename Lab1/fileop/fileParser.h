#ifndef FILEOP_FILE_PARSER_H
#define FILEOP_FILE_PARSER_H

#include <fstream>

class FileParser
{
private:
	ifstream file;
	bool valid = false;
public:
	FileParser(const char *filename);
	~FileParser();

	bool isValid() { return valid; }
};

#endif // FILEOP_FILE_PARSER_H
