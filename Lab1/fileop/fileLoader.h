#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <iostream>
#include <fstream>
#include <string>

class FileLoader {
private:
	std::ifstream fileStream;
	std::istream& file;
	char* data = nullptr;
public:
	FileLoader(std::istream &ifs):file(ifs){}
	FileLoader(const char* filename):fileStream(filename), file(fileStream){}
	FileLoader(const std::string filename):fileStream(filename), file(fileStream){}
	~FileLoader();

	operator char**();
};

#endif // FILE_LOADER_H
