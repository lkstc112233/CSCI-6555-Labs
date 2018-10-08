#ifndef FILEOP_FILE_PARSER_H
#define FILEOP_FILE_PARSER_H

#include <fstream>

/**
 * A parser used to parse script. Equiped with ability to parse numbers and
 * characters.
 */
class FileParser {
 private:
  std::ifstream file;
  bool valid = false;
  template <typename T>
  int tryParseSomething(T *pointer, int count);

 public:
  FileParser(const char *filename);
  ~FileParser();

  bool isValid() { return valid; }

  // for try parse: tries to parse something,
  // Returns how many element is successfully parsed.
  // if succeed parsing every element requested, sets the
  // coresponding pointer to the loaded values.
  // if failed, returns false, and fill the pointer as much as possible.
  // after that, the file pointer will point to as far as it can reach.

  int tryParseFloat(float *pointer, int count = 1);
  int tryParseInt(int *pointer, int count = 1);

  // Expect next character to be c.
  bool expect(char c);

  void restoreFromBad();
};

#endif  // FILEOP_FILE_PARSER_H
