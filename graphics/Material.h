/*
    Created by Alejandro Daniel Noel on 08/12/2016.
*/

#ifndef SIMULATOR_MATERIAL_H
#define SIMULATOR_MATERIAL_H

#include <GL/glew.h>
#include "glm/glm.hpp"

class Material {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLfloat shininess;
    Material() : ambient(glm::vec3(1.0f, 1.0f, 1.0f)),
                 diffuse(glm::vec3(1.0f, 1.0f, 1.0f)),
                 specular(glm::vec3(1.0f, 1.0f, 1.0f)),
                 shininess(1.0f) {}
};

//class MaterialGold : Material {
//    MaterialGold() : ambient(glm::vec3(0.247f, 0.199f, 0.075f)),
//                     diffuse(glm::vec3(0.751f, 0.606f, 0.226f)),
//                     specular(glm::vec3(0.628f, 0.556f, 0.366f)),
//                     shininess(0.4f) {}
//};


#endif //SIMULATOR_MATERIAL_H
