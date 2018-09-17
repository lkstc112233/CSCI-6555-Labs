#include "fileParser.h"

#include <algorithm>
#include <vector>
#include <fstream>
#include <limits>

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

void FileParser::restoreFromBad() {
	if (file) {
		return;
	}
	if (file.eof()) {
		valid = false;
		return;
	}
	file.clear();
	char detect;
	// Skip comments.
	while (file >> detect) {
		if (detect == '#') {
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else {
			file.putback(detect);
			return;
		}
	}
	// Probably eof
	valid = false;
}

int FileParser::tryParseFloat(float *pointer, int count)
{
	std::vector<float> values;
	for (int i = 0; i < count; ++i) {
		if (!isValid()) {
			break;
		}
		float f;
		if (!(file >> f)) {
			restoreFromBad();
			if (!(file >> f)) {
				restoreFromBad();
				break;
			}
		}
		values.push_back(f);
	}
	std::copy(std::begin(values), std::end(values), pointer);
	return values.size();
}