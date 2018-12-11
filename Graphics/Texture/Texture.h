#ifndef GRAPHICS_TEXTURE_TEXTURE_H
#define GRAPHICS_TEXTURE_TEXTURE_H

/**
 * A class managing a 2D texture.
 */
class Texture2D {
 private:
  unsigned id;

 public:
  explicit Texture2D(const char* filename);
  void bind(int position = 0);
};

#endif  // GRAPHICS_TEXTURE_TEXTURE_H