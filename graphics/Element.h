/*
    Created by Alejandro Daniel Noel on 07/12/2016.
*/

#ifndef SIMULATOR_ELEMENT_H
#define SIMULATOR_ELEMENT_H

#include "Material.h"
#include "View.h"

namespace graphics {


class Element {
private:
    GLuint VBO, VAO;
    glm::mat4 M_model;
    std::vector<glm::vec3> vertices;
    Material material;
    Element *parent;
    std::vector<Element*> children;
    View *containerView;

    void setupBuffers();

public:
    Element(std::vector<glm::vec3> &vertices);
    void setParent(Element &parent);
    void addChild(Element &child);
    void render();
    void translate(GLfloat x, GLfloat y, GLfloat z);
    void rotateX(GLfloat angle);
    void rotateY(GLfloat angle);
    void rotateZ(GLfloat angle);
    void setContainerView(View &containerView);
    glm::mat4 getWorldPos();
};

} // Namespace graphics

#endif //SIMULATOR_ELEMENT_H