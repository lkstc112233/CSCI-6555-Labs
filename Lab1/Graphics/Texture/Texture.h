#ifndef GRAPHICS_TEXTURE_TEXTURE_H
#define GRAPHICS_TEXTURE_TEXTURE_H

class Texture2D {
private:
    unsigned id;
public:
    Texture2D(const char* filename);
    void bind(int position = 0);
};

#endif // GRAPHICS_TEXTURE_TEXTURE_H