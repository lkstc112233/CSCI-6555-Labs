#include "fileLoader.h"

#include <iostream>

FileLoader::~FileLoader() {
	delete[] data;
}

FileLoader::operator char**() {
	if(!file.good()) {
		return nullptr;
	}
    
    file.seekg(0, std::ios::end);
    unsigned long length = file.tellg();
    file.seekg(std::ios::beg);

    data = new char[length + 1];
    data[length] = 0;

    unsigned int i = 0;
	while (file.good()) {
	    data[i] = file.get();
	    if (!file.eof()) {
	    	i++;
	    }
	}
	    
	data[i] = 0;

	return &data;
}