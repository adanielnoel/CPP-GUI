/*
    Created by Alejandro Daniel Noel on 27/12/2016.
*/

#ifndef SIMULATOR_GEOMETRYPRIMITIVE_H
#define SIMULATOR_GEOMETRYPRIMITIVE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"
#include "Primitives.h"
#include <vector>

namespace graphics {
// Scroll to the end for drawing functions

class Path {
public:
    virtual void draw();
    inline void setFilled(bool flag) {filled = flag;}
    inline void setFillColor(Color color) {fillColor = color;}
    inline void setFillColor(float r, float g, float b, float a=1.f) {setFillColor(Color(r, g, b, a));}
    inline void setStroked(bool flag) {stroked = flag;}
    inline void setStrokeThickness(float thickness) {strokeThickness = thickness;}
    inline void setStrokeColor(Color color) {strokeColor = color;}
    inline void setStrokeColor(float r, float g, float b, float a=1.f) {setStrokeColor(Color(r, g, b, a));}

protected:
    bool filled = true;
    bool stroked = true;
    Color fillColor = {0.2f, 0.2f, 0.2f, 0.8f};
    float strokeThickness = 1.f;
    Color strokeColor = {0.f, 0.f, 0.f, 1.f};
    std::vector<glm::vec3> vertices;
    virtual inline void addVertex(float x, float y) {
        vertices.push_back(glm::vec3(x, y, 0.f));
        vertexBufferUpToDate = false;
    }
    virtual inline void addVertex(glm::vec2 v) {
        addVertex(v.x, v.y);
    }
    virtual inline void clearVertices() {
        vertices.clear();
        vertexBufferUpToDate = false;
    }
    virtual void updateVertexBuffer(std::vector<glm::vec2> texCoordinates = {});
    GLuint VBO = (GLuint)-1;
    GLuint VAO = (GLuint)-1;
    bool vertexBufferUpToDate = false;
};

class Line : public Path {
public:
    Line(){stroked= false;};
    Line(glm::vec2 p1, glm::vec2 p2, float thickness);
    void setVertices(glm::vec2 p1, glm::vec2 p2);
    inline void clearVertices() {Path::clearVertices();}
    float thickness;
};

class Rectangle : public Path {
public:
    Rectangle(float x, float y, float w, float h) : Rectangle(Rect(x, y, w, h)) {};
    Rectangle(Rect rect) {setRect(rect);}
    Rectangle() {};

    virtual inline void setRect(Rect rect) {
        clearVertices();
        addVertex(rect.x, rect.y);
        addVertex(rect.x, rect.y + rect.h);
        addVertex(rect.x + rect.w, rect.y + rect.h);
        addVertex(rect.x + rect.w, rect.y);
    }
};

class TexturedRectangle : public Rectangle{
public:
    TexturedRectangle(Rect rect, GLuint gl_texture) {
        setRect(rect);
        textureID = gl_texture;
    };
    TexturedRectangle() : Rectangle(){};

    inline void setTexture(GLuint gl_texture) {textureID = gl_texture;}
    void draw();

private:
    GLuint textureID;
};

class RoundedRectangle : public Rectangle {
public:
    RoundedRectangle(Rect rect, float radius){
        this->radius = radius;
        setRect(rect);
    };
    void setRect(Rect rect);

private:
    float radius;
};


/**
 * Quick function to draw rectangles without having to create an instance of Rectangle
 * @param rect
 * @param fill
 * @param stroke
 */
void drawRectangle(Rect rect, Color fill = Color(), Color stroke = Color(0.f, 0.f, 0.f, 1.f));

/**
 * Quick function to draw textures on a rectangle
 * @param rect
 * @param gl_texture
 */
void drawTexture(Rect rect, GLuint gl_texture);

void drawLine(glm::vec2 p1, glm::vec2 p2, float thickness, Color fill = Color());

} /* namespace graphics*/

#endif //SIMULATOR_GEOMETRYPRIMITIVE_H
