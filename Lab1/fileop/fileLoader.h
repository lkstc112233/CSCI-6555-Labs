#ifndef FILEOP_FILE_LOADER_H
#define FILEOP_FILE_LOADER_H

#include <iostream>

class FileLoader
{
private:
	char *data = nullptr;
public:
	FileLoader(const char *filename);
	~FileLoader();

	operator char **();
};

#endif // FILEOP_FILE_LOADER_H
