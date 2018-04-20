/*
    Created by Alejandro Daniel Noel on 05/01/2017.
*/

#ifndef SIMULATOR_TEXT_H
#define SIMULATOR_TEXT_H

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include "glm/glm.hpp"
#include "ft2build.h"
#include FT_FREETYPE_H

namespace graphics {

/// Loads font and generates rasterised of the character sets 
class Font;
struct Character;

class Text {
private:
    static GLuint VAO;
    static GLuint VBO;
public:
    Font *font;
    std::string string;
    Text(std::string string, Font *font);

    Text(std::string string);
    void put(float x, float y, GLfloat height, glm::vec3 color);
};


/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint textureID;    ///< ID handle of the glyph texture
    glm::ivec2 size;     ///< Size of glyph
    glm::ivec2 bearing;  ///< Offset from baseline to left/top of glyph
    GLuint advance;      ///< Horizontal offset to advance to next glyph
};

class Font {
private:
    bool initialised;
    GLfloat height;
    std::map<GLchar, Character> characters;

public:
    Font() {};
    /**
     * Reads font file and generates the font faces (vector image)
     * @param fontSource: path to font file (.ttf)
     */
    Font(std::string fontSource, GLfloat renderHeight = 40);
    Character getChar(GLchar charCode);
    inline bool isInitialised() {return initialised;}
    inline GLfloat getHeight() { return height;}
};

} /* namespace graphics*/

#endif //SIMULATOR_TEXT_H
