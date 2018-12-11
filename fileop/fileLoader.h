#ifndef FILEOP_FILE_LOADER_H
#define FILEOP_FILE_LOADER_H

#include <iostream>

/**
 * A class loads and holds a file as text into a char array.
 * Specially designed for loading shader.
 */
class FileLoader {
 private:
  char *data = nullptr;

 public:
  FileLoader(const char *filename);
  ~FileLoader();

  operator char **();
};

#endif  // FILEOP_FILE_LOADER_H
