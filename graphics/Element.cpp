/*
    Created by Alejandro Daniel Noel on 07/12/2016.
*/

#include "Element.h"
#include "Window.h"
#include "ResourceManager.h"

namespace graphics {

Element::Element(std::vector<glm::vec3> &vertices) {
    this->vertices = vertices;
    setupBuffers();
}

void Element::render() {
    if (vertices.size()) {
        glm::mat4 model_matrix = getWorldPos();
        resourceManager.shader(COLOR_3D_SHADER).sendModelMatrix(model_matrix);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3 * (GLsizei) vertices.size());
        //    glBindVertexArray(0);
    }
    for (auto child : children){
        child->render();
    }
}

void Element::translate(GLfloat x, GLfloat y, GLfloat z) {
    M_model = glm::translate(M_model, glm::vec3(x, y, z));
}

void Element::rotateX(GLfloat angle) {
    M_model = glm::rotate(M_model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
}

void Element::rotateY(GLfloat angle) {
    M_model = glm::rotate(M_model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Element::rotateZ(GLfloat angle) {
    M_model = glm::rotate(M_model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Element::setupBuffers() {
    // Do not set buffers if this is an empty object (just a coordinate system for other objects)
    if (vertices.size() == 0)
        return;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(GLfloat), &vertices.front(), GL_STATIC_DRAW);
    //GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
    //GL_DYNAMIC_DRAW: the data is likely to change a lot.
    //GL_STREAM_DRAW: the data will change every time it is drawn.

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Element::setParent(Element &parent) {
    this->parent = &parent;
    parent.addChild(*this);
}

void Element::addChild(Element &child) {
    // Add only if not already added
    if(std::find(children.begin(), children.end(), &child) == children.end())
        this->children.push_back(&child);
}

glm::mat4 Element::getWorldPos() {
    if(parent) {
        return parent->getWorldPos() * M_model;
    } else {
        return M_model;
    }
}

void Element::setContainerView(View &containerView) {
    this->containerView = &containerView;
    for(auto element : children){
        element->setContainerView(containerView);
    }
}


} /* namespace graphics*/