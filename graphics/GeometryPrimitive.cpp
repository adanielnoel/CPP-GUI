/*
    Created by Alejandro Daniel Noel on 27/12/2016.
*/

#include "GeometryPrimitive.h"
#include "ResourceManager.h"
#include <cmath>

namespace graphics {

void Path::draw() {
    if (!vertexBufferUpToDate) updateVertexBuffer();
    resourceManager.useShader(COLOR_2D_SHADER);
    glBindVertexArray(VAO);
    if(filled) {
        resourceManager.shader(COLOR_2D_SHADER).sendColorVec(fillColor.getVector());
        glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei) vertices.size());
    }
    if (stroked){
        // TODO: Implement proper line rendering. Check outlining techniques and line rendering techniques
        resourceManager.shader(COLOR_2D_SHADER).sendColorVec(strokeColor.getVector());
        for (unsigned int i = 1; i < vertices.size(); i++) {
            drawLine(vertices.at(i-1), vertices.at(i), 1.0, strokeColor);
        }
        drawLine(vertices.back(), vertices.front(), 1.0, strokeColor);
//        glDrawArrays(GL_LINE_LOOP, 0, (GLsizei) vertices.size());
    }
}

void Path::updateVertexBuffer(std::vector<glm::vec2> texCoordinates) {
    if (VAO == -1) glGenVertexArrays(1, &VAO);
    if (VBO == -1) glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if (texCoordinates.empty()) {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(GLfloat), &vertices.front(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    }
    else if (texCoordinates.size() == vertices.size()){
//        std::vector
        GLfloat coords[] = {vertices[0][0], vertices[0][1], 0.f, texCoordinates[0][0], texCoordinates[0][1],
                            vertices[1][0], vertices[1][1], 0.f, texCoordinates[1][0], texCoordinates[1][1],
                            vertices[2][0], vertices[2][1], 0.f, texCoordinates[2][0], texCoordinates[2][1],
                            vertices[3][0], vertices[3][1], 0.f, texCoordinates[3][0], texCoordinates[3][1]};
        glBufferData(GL_ARRAY_BUFFER, sizeof(coords), &coords, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    }
    else {
        throw std::invalid_argument( "number of texture coordinates does not match number of vertices" );
    }

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    vertexBufferUpToDate = true;
}

void TexturedRectangle::draw() {
    if (!vertexBufferUpToDate) {
        std::vector<glm::vec2> texCoords= {{0.f, 1.f},   // Top-left
                                           {0.f, 0.f},   // Bottom-left
                                           {1.f, 0.f},   // Bottom-right
                                           {1.f, 1.f}};  // Top-right
        updateVertexBuffer(texCoords);
    }
    resourceManager.useShader(TEXTURE_2D_SHADER);

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}


void RoundedRectangle::setRect(Rect rect) {
    clearVertices();
    int numberOfSteps = radius >= 4 ? (int)radius/4 : (int)radius;
    // upper-left corner
    for(int i = 0; i <= numberOfSteps; i++) {
        double angle = M_PI * i / (numberOfSteps * 2);
        addVertex(float(rect.x + radius * (1 - std::sin(angle))),
                  float(rect.y + radius * (1 - std::cos(angle))));
    }
    // lower-left corner
    for(int i = 0; i <= numberOfSteps; i++){
        double angle = M_PI * i / (numberOfSteps * 2);
        addVertex(float(rect.x + radius * (1 - std::cos(angle))),
                  float(rect.y + rect.h + radius * (std::sin(angle) - 1)));
    }
    // lower-right corner
    for(int i = 0; i <= numberOfSteps; i++){
        double angle = M_PI * i / (numberOfSteps * 2);
        addVertex(float(rect.x + rect.w + radius * (std::sin(angle) - 1)),
                  float(rect.y + rect.h + radius * (std::cos(angle) - 1)));
    }
    // upper-right corner
    for(int i = 0; i <= numberOfSteps; i++){
        double angle = M_PI * i / (numberOfSteps * 2);
        addVertex(float(rect.x + rect.w + radius * (std::cos(angle) - 1)),
                  float(rect.y + radius * (1 - std::sin(angle))));
    }
}


void drawRectangle(Rect rect, Color fill, Color stroke) {
    static Rectangle rectangle;
    rectangle.setRect(rect);
    rectangle.setFillColor(fill);
    rectangle.setStrokeColor(stroke);
    rectangle.draw();
}

void drawTexture(Rect rect, GLuint gl_texture) {
    static TexturedRectangle rectangle;
    rectangle.setRect(rect);
    rectangle.setTexture(gl_texture);
    rectangle.draw();
}


void drawLine(glm::vec2 p1, glm::vec2 p2, float thickness, Color fill){
    static Line line;
    line.clearVertices();
    line.thickness = thickness;
    line.setVertices(p1, p2);
    line.draw();
}


Line::Line(glm::vec2 p1, glm::vec2 p2, float thickness) {
    setVertices(p1, p2);
    this->thickness = thickness;
    stroked = false;
};

void Line::setVertices(glm::vec2 p1, glm::vec2 p2) {

    // Compute line direction
    glm::vec2 normal = p2 - p1;

    // Normalise line direction and make half size
    normal *= 0.5 * thickness / glm::length(normal);

    // Make normal perpendiculars of the line
    glm::vec2 perpendicular = {normal.y, -normal.x};

    addVertex(p1 + perpendicular);
    addVertex(p2 + perpendicular);
    addVertex(p2 - perpendicular);
    addVertex(p1 - perpendicular);
}

} /* namespace graphics*/