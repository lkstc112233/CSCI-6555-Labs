#include "fileLoader.h"

#include <fstream>

FileLoader::FileLoader(const char *filename) {
	std::ifstream file(filename);
	if (!file.good())
	{
		return;
	}

	file.seekg(0, std::ios::end);
	unsigned long length = file.tellg();
	file.seekg(std::ios::beg);

	data = new char[length + 1];
	data[length] = 0;

	unsigned int i = 0;
	while (file.good())
	{
		data[i] = file.get();
		if (!file.eof())
		{
			i++;
		}
	}

	data[i] = 0;
}

FileLoader::~FileLoader()
{
	delete[] data;
}

FileLoader::operator char **()
{
	return &data;
}