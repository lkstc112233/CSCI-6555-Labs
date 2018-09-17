#include <iostream>

#include "fileop/fileParser.h"

int main(int argc, char** argv) {
	if (argc < 2) {
		return 1;
	}

	FileParser parser(argv[1]);
	int count;
	if (parser.tryParseInt(&count)) {
		for (int i = 0; i < count; ++i) {
			if (!parser.expect('('))
			{
				std::cerr << "Invalid syntax." << std::endl;
			}
			float data[7];
			int loaded = parser.tryParseFloat(data, 7);
			if (!parser.expect(')'))
			{
				std::cerr << "Invalid syntax." << std::endl;
			}
			for (int j = 0; j < loaded; ++j) {
				std::cout << data[j] << ", ";
			}
			std::cout << std::endl;
		}
	}


	return 0;
}