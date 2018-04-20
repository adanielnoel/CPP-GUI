/*
    Created by Alejandro Daniel Noel on 05/01/2017.
*/

#include "Text.h"
#include <iostream>
#include "Shader.h"
#include "ResourceManager.h"

namespace graphics {

GLuint Text::VAO = (GLuint) -1;
GLuint Text::VBO = (GLuint) -1;


Text::Text(std::string string, Font *font) : string(string), font(font) {
    if (VAO == (GLuint) -1) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

Text::Text(std::string string){
    new (this) Text(string, resourceManager.defaultFont());
}

void Text::put(float x, float y, GLfloat height, glm::vec3 color) {
    if (!font->isInitialised()) {
        std::cout << "Cannot render text, font is not initialised" << std::endl;
        return;
    }

    resourceManager.shader(TEXT_2D_SHADER).use();
    resourceManager.shader(TEXT_2D_SHADER).sendColorVec(color.r, color.g, color.b);
    glBindVertexArray(VAO);

    GLfloat scale = height/font->getHeight();
    std::string::const_iterator c;
    for (c = string.begin(); c != string.end(); c++) {
        Character ch = font->getChar(*c);

        GLfloat xpos = x + ch.bearing.x * scale;
        GLfloat ypos = y - ch.bearing.y * scale;

        GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 1.0 },
                { xpos,     ypos,       0.0, 0.0 },
                { xpos + w, ypos,       1.0, 0.0 },

                { xpos,     ypos + h,   0.0, 1.0 },
                { xpos + w, ypos,       1.0, 0.0 },
                { xpos + w, ypos + h,   1.0, 1.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


Font::Font(std::string fontSource, GLfloat renderHeight) {
    height = renderHeight;
    initialised = false;
    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, fontSource.c_str(), 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, FT_UInt(renderHeight));


    // Disable byte-alignment restriction, otherwise OpenGL requires the textures to have a number of bytes multiple
    // of 4, which may not be the case since the characters are loaded as 8 bit textures and therefore their size
    // multiplicity is dependant only on their dimensions
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use

        Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<GLuint>(face->glyph->advance.x)
        };
        characters[c] = character;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    initialised = true;
    return ;
}

Character Font::getChar(GLchar charCode) {
    return characters[charCode];
}


} /* namespace graphics*/