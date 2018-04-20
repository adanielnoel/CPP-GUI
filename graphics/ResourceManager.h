/*
    Created by Alejandro Daniel Noel on 03/01/2017.
*/

#ifndef SIMULATOR_RESOURCEMANAGER_H
#define SIMULATOR_RESOURCEMANAGER_H

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "glm/glm.hpp"
#include "Text.h"
#include "Shader.h"
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include "Window.h"

#define COLOR_SCHEME_DARK "./resources/JSON/color_scheme_dark.json"

namespace graphics {

struct Settings {
    int pixelScaling = 1;
    int multisamplingLevel = 4;
};

class ResourceManager;
extern ResourceManager resourceManager;

class ResourceManager {
private:
    std::vector<Window *> windows;
    std::map<std::string, glm::vec3> colors;
    std::map<std::string, glm::vec4> colorScheme;
    std::map<std::string, Font> fonts;
    std::map<ShaderType, Shader> shaders;
    bool shadersLoaded = false;

public:
    ResourceManager();

    /**
     * Initialises graphics resources
     */
    void init();

    /**
     * Starts the main graphics loop. Should be called in `main()` after initialisation code.
     * Note that this is a blocking function ---i.e. runs forever until app is closed.
     */
    void cycle();

    /**
     *
     * @return `true` if there are one or more windows open, else returns false.
     */
    bool running();

    void addWindow(Window &newWindow);
    void deleteWindow(Window *win);

    /**
     * @return sequentially unique IDs. Each call increments an internal counter.
     */
    int getUniqueID();

    /**
     * Handles passed event if supported, otherwise ignores it. Note that an event is first are passed to the
     * relevant views in the view tree. The event is propagated through the responder chain until it is handled
     * or otherwise ends its journey in this function.
     *
     * @param event to be handled
     *
     */
    void dispatchEvent(Event *event);

    /**
     * Finds the color of the requested feature in the currently bound color scheme.
     * Throws en exception if the feature is not found
     * @param feature is the name of the feature to be searched and should be equal to how it appears in
     * the color scheme json file
     * @return a vector with the corresponding RGBA color components.
     */
    glm::vec4 colorForFeature(std::string feature);

    glm::mat4 current_projection_matrix;
    float current_dpi_scaling;

    void loadColorScheme(std::string filePath);
    void loadFont(std::string fontSource);
    void loadShaders();
    inline bool areShadersLoaded() { return shadersLoaded;};

    /**
     * Similar to the function use() in the Shader class, but also sends the current projection matrix.
     * @param shaderType
     */
    void useShader(ShaderType shaderType);

    Shader &shader(ShaderType shaderType);
    Font *defaultFont();
};

} /* namespace graphics*/

#endif //SIMULATOR_RESOURCEMANAGER_H
