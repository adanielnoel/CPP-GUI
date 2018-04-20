/*
    Created by Alejandro Daniel Noel on 03/01/2017.
*/

#include "ResourceManager.h"
#include "Window.h"

//std::map<std::string, glm::vec3> ResourceManager::colors;
//std::map<std::string, glm::vec4> ResourceManager::colorScheme;
//std::map<std::string, Font> ResourceManager::fonts;
//std::map<ShaderType, Shader> ResourceManager::shaders;

namespace graphics{

ResourceManager resourceManager;    // Global object withing the graphics:: namespace

ResourceManager::ResourceManager() {
}

void ResourceManager::init() {
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return;
    }
    resourceManager.loadColorScheme(COLOR_SCHEME_DARK);
}

void ResourceManager::loadColorScheme(std::string filePath) {
    std::ifstream file(filePath);
    assert(file.good());
    std::string str;
    std::string file_contents;
    while (std::getline(file, str))
    {
        file_contents += str + '\n';
    }
    rapidjson::Document document;
    document.Parse(file_contents.c_str());

    colors.clear();
    colorScheme.clear();

    // Read defined colors
    const rapidjson::Value& JSON_colors = document["colorsArray"];
    assert(JSON_colors.IsArray());
    for (rapidjson::SizeType i = 0; i < JSON_colors.Size(); i++) {
        colors[JSON_colors[i]["colorName"].GetString()] = glm::vec3(JSON_colors[i]["r"].GetFloat(),
                                                                    JSON_colors[i]["g"].GetFloat(),
                                                                    JSON_colors[i]["b"].GetFloat());
    }

    // Read color scheme
    const rapidjson::Value& JSON_colorScheme = document["colorScheme"];
    assert(JSON_colorScheme.IsObject());
    for (rapidjson::Value::ConstMemberIterator itr = JSON_colorScheme.MemberBegin();
         itr != JSON_colorScheme.MemberEnd(); ++itr) {
        std::string colorStr = itr->value["color"].GetString();
        // Check if the specified color has been defined and assert if not
        assert(colors.find(colorStr) != colors.end());
        colorScheme[itr->name.GetString()] = glm::vec4(colors[colorStr],
                                                       itr->value["alpha"].GetFloat());

    }

//    for (auto item : colorScheme){
//        std::cout << item.first << " color: (" << item.second.r << ", " << item.second.g << ", " << item.second.b << ", " << item.second.a << ")" << std::endl;
//    }
}


glm::vec4 ResourceManager::colorForFeature(std::string feature) {
    if(colorScheme.find(feature) == colorScheme.end()){
        throw std::runtime_error("Feature \"" + feature + "\" not found in color scheme");
    }
    return colorScheme[feature];
}

void ResourceManager::loadFont(std::string fontSource) {
    Font font(fontSource);
    // Font name added to the font dictionary without the extension
    fonts[fontSource.substr(0, fontSource.find_last_of("."))] = font;
}

void ResourceManager::useShader(ShaderType shaderType) {
//    Shader the_shader = shader(shaderType);
//    the_shader.use();
//    the_shader.sendProjectionMatrix(current_projection_matrix);
//    shader(shaderType).use();
    shader(shaderType).sendDPIScaling(current_dpi_scaling);
    shader(shaderType).sendProjectionMatrix(current_projection_matrix);
    // shaderType.use() is implicitly called by any uniform-sending function
}

Shader &ResourceManager::shader(ShaderType shaderType) {
    return shaders[shaderType];
}

void ResourceManager::loadShaders() {
    if (!shadersLoaded) {
        shaders[COLOR_2D_SHADER] = Shader(COLOR_2D_SHADER, 
                                          "./resources/shaders/shader_2D_colour.vert",
                                          "./resources/shaders/shader_2D_colour.frag");
        shaders[COLOR_2D_LINE_SHADER] = Shader(COLOR_2D_LINE_SHADER,
                                               "./resources/shaders/shader_2D_colour_line.vert",
                                               "./resources/shaders/shader_2D_colour_line.frag",
                                               "./resources/shaders/shader_2D_colour_line.geom");
        shaders[COLOR_3D_SHADER] = Shader(COLOR_3D_SHADER, 
                                          "./resources/shaders/shader_3D_colour.vert",
                                          "./resources/shaders/shader_3D_colour.frag");
        shaders[TEXTURE_2D_SHADER] = Shader(TEXTURE_2D_SHADER, 
                                            "./resources/shaders/shader_2D_texture.vert",
                                            "./resources/shaders/shader_2D_texture.frag");
        shaders[MATERIAL_3D_SHADER] = Shader(MATERIAL_3D_SHADER,
                                             "./resources/shaders/shader_3D_material.vert",
                                             "./resources/shaders/shader_3D_material.frag");
        shaders[TEXT_2D_SHADER] = Shader(TEXT_2D_SHADER, 
                                         "./resources/shaders/shader_2D_text.vert", 
                                         "./resources/shaders/shader_2D_text.frag");
        shadersLoaded = true;
    }
}

void ResourceManager::addWindow(Window &newWindow) {
    newWindow.setID(getUniqueID());
    windows.push_back(&newWindow);
}

void ResourceManager::deleteWindow(Window *win) {
    windows.erase(std::find(windows.begin(), windows.end(), win));
}

int ResourceManager::getUniqueID() {
    static int ID = 0;
    ID ++;
    return ID;
}

void ResourceManager::cycle() {
    for (auto& win : windows) {
        current_dpi_scaling = win->getDPIScaling();
        win->update();
        if (win->isClosed()) {
            deleteWindow(win);
            break; // To avoid out-of-range exceptions
        }
    }
}

bool ResourceManager::running() {
    return (bool)windows.size();
}

void ResourceManager::dispatchEvent(Event *event) {
    // Ignore move events to keep output clean
    if (event->type() == EVENT_MOUSE_MOVE) return;
    std::cout << event->description() << " discarded by application" << std::endl;
}

Font *ResourceManager::defaultFont() {
    static Font defFT("./resources/fonts/HelveticaNeue.ttf");
    return &defFT;
}


} /* namespace graphics*/