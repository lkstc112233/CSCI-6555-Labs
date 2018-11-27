#include "fileParser.h"

#include <algorithm>
#include <fstream>
#include <limits>
#include <string>
#include <vector>

FileParser::FileParser(const char *filename) : file(filename) {
  if (file) {
    valid = true;
  }
}

FileParser::~FileParser() { file.close(); }

std::string FileParser::parseString() {
  std::string result;

  // Skip comments
  restoreFromBad();

  file >> result;
  return result;
}

void FileParser::restoreFromBad() {
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
    } else {
      file.putback(detect);
      return;
    }
  }
  // Probably eof
  valid = false;
}

template <typename T>
int FileParser::tryParseSomething(T *pointer, int count) {
  std::vector<T> values;
  for (int i = 0; i < count; ++i) {
    if (!isValid()) {
      break;
    }
    T receiver;
    if (!(file >> receiver)) {
      restoreFromBad();
      if (!(file >> receiver)) {
        restoreFromBad();
        break;
      }
    }
    values.emplace_back(receiver);
  }
  std::copy(std::begin(values), std::end(values), pointer);
  return values.size();
}

int FileParser::tryParseFloat(float *pointer, int count) {
  return tryParseSomething(pointer, count);
}

int FileParser::tryParseInt(int *pointer, int count) {
  return tryParseSomething(pointer, count);
}

bool FileParser::expect(char c) {
  char receiver;
  if (file >> receiver) {
    if (receiver == '#') {
      file.putback(receiver);
      restoreFromBad();
      return expect(c);
    }
    if (c == receiver) {
      return true;
    }
    file.putback(receiver);
    return false;
  }
  restoreFromBad();
  if (!isValid()) {
    return false;
  }
  return expect(c);
}
