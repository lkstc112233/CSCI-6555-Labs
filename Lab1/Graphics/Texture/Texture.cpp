#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Texture.h"

#include "../Libraries/stb_image.h"

Texture2D::Texture2D(const char *filename)
{
    int width, height, notused;
    stbi_set_flip_vertically_on_load(true);  
    unsigned char *loadedTextureData = stbi_load(filename, &width, &height, &notused, 0);
    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, loadedTextureData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(loadedTextureData);
}

void Texture2D::bind(int position) {
    glActiveTexture(GL_TEXTURE0 + position);
	glBindTexture(GL_TEXTURE_2D, id);
}