
#include "Shader.h"

namespace graphics {


ShaderType Shader::currentShader = UNKNOWN_SHADER;


void Shader::sendModelMatrix(glm::mat4 model_matrix) {
    use();
    glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
}

void Shader::sendViewMatrix(glm::mat4 view_matrix) {
    use();
    glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));
}

void Shader::sendProjectionMatrix(glm::mat4 projection_matrix) {
    use();
    glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
}

void Shader::sendTransformMatrix(glm::mat4 transform_matrix) {
    use();
    glUniformMatrix4fv(transform_matrix_loc, 1, GL_FALSE, glm::value_ptr(transform_matrix));
}

void Shader::sendColorVec(glm::vec4 color_vec) {
    use();
    glUniform4fv(color_vec_loc, 1, glm::value_ptr(color_vec));
}

void Shader::sendColorVec(float r, float g, float b, float a) {
    sendColorVec(glm::vec4(r, g, b, a));
}

void Shader::sendZ(int z) {
    use();
    glUniform1f(z_coordinate_loc, (GLfloat)z / INT_MAX);
}

void Shader::sendDPIScaling(GLfloat DPIscaling) {
    use();
    glUniform1f(dpi_scaling_loc, DPIscaling);
}


void Shader::use() {
    if (Shader::currentShader != thisShader) {
        #ifdef SHADER_DEBUGGING
        std::cout << "Switching to \"" << descriptionForShader(thisShader) << "\"    (was \"" << descriptionForShader(currentShader) << "\")" << std::endl;
        #endif
        glUseProgram(this->program);
        currentShader = thisShader;
    }
}

Shader::Shader(ShaderType type, std::string vertexFile, std::string fragmentFile, std::string geometryFile) {
    thisShader = type;

    GLuint sVertex, sFragment, gShader;
    // Vertex Shader
    std::ifstream ifs(vertexFile);
    if (!ifs.good()){
        std::cout << vertexFile << " does not exist" << std::endl;
    }
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );
    const char* vertexSource = content.c_str();
    ifs.close();
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, NULL);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");

    // Fragment Shader
    ifs.open(fragmentFile);
    if (!ifs.good()){
        std::cout << fragmentFile << " does not exist" << std::endl;
    }
    content = std::string( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );
    const char* fragmentSource = content.c_str();
    ifs.close();
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, NULL);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");

    // If geometry shader source code is given, also compile geometry shader
    if (!geometryFile.empty())
    {
        ifs.open(geometryFile);
        if (!ifs.good()){
            std::cout << geometryFile << " does not exist" << std::endl;
        }
        content = std::string( (std::istreambuf_iterator<char>(ifs) ),
                               (std::istreambuf_iterator<char>()    ) );
        const char* geometrySource = content.c_str();
        ifs.close();
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geometrySource, NULL);
        glCompileShader(gShader);
        checkCompileErrors(gShader, "GEOMETRY");
    }
    // Shader Program
    this->program = glCreateProgram();
    glAttachShader(this->program, sVertex);
    glAttachShader(this->program, sFragment);
    if (!geometryFile.empty())
        glAttachShader(this->program, gShader);
    glLinkProgram(this->program);
    checkCompileErrors(this->program, "PROGRAM");
    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (!geometryFile.empty())
        glDeleteShader(gShader);

    readUniformLocations();
}

void Shader::checkCompileErrors(GLuint object, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- "
                      << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "ERROR::Shader: Link-time error: Type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- "
                      << std::endl;
        }
    }
}

std::string Shader::descriptionForShader(ShaderType shaderType) {
    switch (shaderType) {
        case COLOR_2D_SHADER:
            return "color 2D shader";
        case COLOR_2D_LINE_SHADER:
            return "color 2D line shader";
        case COLOR_3D_SHADER:
            return "color 3D shader";
        case TEXTURE_2D_SHADER:
            return "texture 2D shader";
        case MATERIAL_3D_SHADER:
            return "material 3D shader";
        case TEXT_2D_SHADER:
            return "text 2D shader";
        default:
            return "unknown shader";
    }
}

void Shader::readUniformLocations() {
    model_matrix_loc = glGetUniformLocation(this->program, "model_matrix");
    view_matrix_loc = glGetUniformLocation(this->program, "view_matrix");
    projection_matrix_loc = glGetUniformLocation(this->program, "projection_matrix");
    transform_matrix_loc = glGetUniformLocation(this->program, "transform_matrix");
    color_vec_loc = glGetUniformLocation(this->program, "color_vec");
    z_coordinate_loc = glGetUniformLocation(this->program, "z_coordinate");
    dpi_scaling_loc = glGetUniformLocation(this->program, "dpi_scaling");

    #ifdef SHADER_DEBUGGING
    std::cout << "Uniform locations for shader \"" + descriptionForShader(thisShader) + "\"" << std::endl;
    std::cout << "\tmodel_matrix_loc: " << model_matrix_loc << std::endl;
    std::cout << "\tview_matrix_loc: " << view_matrix_loc << std::endl;
    std::cout << "\tprojection_matrix_loc: " << projection_matrix_loc << std::endl;
    std::cout << "\ttransform_matrix_loc: " << transform_matrix_loc << std::endl;
    std::cout << "\tcolor_vec_loc: " << color_vec_loc << std::endl;
    std::cout << "\tz_coordinate_loc: " << z_coordinate_loc << std::endl;
    std::cout << "\tdpi_scaling_loc: " << dpi_scaling_loc << std::endl;
    #endif
}

} /* namespace graphics*/