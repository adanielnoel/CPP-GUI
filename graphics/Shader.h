//
//  Shader.h
//  Graphics
//
//  Created by Alejandro Daniel Noel on 15/11/2016.
//
//

#ifndef Shader_h
#define Shader_h

// Uncomment for verbose output on shader usage
#define SHADER_DEBUGGING

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <initializer_list>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
namespace graphics {

// Remember to also add them to Shader::shaders and write initialisation in the resource manager
enum ShaderType {
    COLOR_2D_SHADER,
    COLOR_2D_LINE_SHADER,
    COLOR_3D_SHADER,
    TEXTURE_2D_SHADER,
    MATERIAL_3D_SHADER,
    TEXT_2D_SHADER,
    UNKNOWN_SHADER
};

/**
 *  General class for shader creation and management.
 *  Notes:
 *      Every new shader needs to be added in th ShaderType enum
 *      Every new uniform needs to be registered by:
 *          1) Creating a location int value in the private section
 *          2) Reading the location in readUniformLocations()
 *          3) Creating a function to send values to it
 *          4) Adding it to the ResourceManager `shaders` map
 */
class Shader {
private:
    // Checks if compilation or linking failed and if so, print the error logs
    void    checkCompileErrors(GLuint object, std::string type);
    static ShaderType currentShader;
    ShaderType thisShader;

    int model_matrix_loc;
    int view_matrix_loc;
    int projection_matrix_loc;
    int transform_matrix_loc;
    int color_vec_loc;
    int z_coordinate_loc;
    int dpi_scaling_loc;
    void readUniformLocations();

protected:
    GLuint program;

public:
    Shader() {}
    Shader(ShaderType type,
           std::string vertexFile,
           std::string fragmentFile,
           std::string geometryFile = ""); // Note: geometry source code is optional
    static std::string descriptionForShader(ShaderType shaderType);

    // Uniform sender functions
    void sendModelMatrix(glm::mat4 model_matrix);
    void sendViewMatrix(glm::mat4 view_matrix);
    void sendProjectionMatrix(glm::mat4 projection_matrix);
    void sendTransformMatrix(glm::mat4 transform_matrix);
    void sendZ(int z);
    void sendColorVec(glm::vec4 color_vec);
    void sendColorVec(float r, float g, float b, float a = 1.0);
    void sendDPIScaling(GLfloat DPIscaling);

    void use();
};

class ShaderGroup {
private:
    std::map<ShaderType, Shader> shaders;
};

} /* namespace graphics*/
#endif /* Shader_h */
