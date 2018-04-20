/*
    Created by Alejandro Daniel Noel on 26/11/2016.
*/

#ifndef GRAPHICS_WINDOW_H
#define GRAPHICS_WINDOW_H

#include "View.h"
#include "Shader.h"
#include "Event.h"

namespace graphics {

class Window {
public:
//    static auto glfw_error_callback (int error, const char* description);
    Window(int width, int height, std::string title);
    Window(int width, int height): Window(width, height, "") {}
    Window() : Window(800, 400, "") {}
    ~Window();
    void didResize(int newWidth, int newHeight);
    View &getView();
    void update();
    bool isClosed() { return closed;};
    void updateFPS();
    void setFPS(double required_fps);
    Size getSize();
    void setID(int ID);
    void setNeedsRedraw();
    void getMousePos(int *x, int *y);
    float getDPIScaling();
    inline int getFrameCount() { return frameCount;};
    View *firstResponder = nullptr;

private:
    int width, height;
    std::string title;
    float dpi_scaling;    // For high DPI displays (on my macbook retina 1 pixel = 2 points)
    int ID;
    View containerView;
    bool closed;

    // Frame counting and throttling
    double actual_fps;
    double required_fps;
    double timeBetweenUpdates;
    int frameCount;
    double timeOfLastUpdate;

    bool needsRedraw;

    void draw();
    void dispatchEvent(Event *event);
    static void resize_callback(GLFWwindow* gl_win, int width, int height);
    static void key_callback(GLFWwindow* gl_win, int key, int scancode, int action, int mode);
    static void character_callback(GLFWwindow* gl_win, unsigned int UTF_char);
    static void mouse_button_callback(GLFWwindow *gl_win, int button, int action, int mods);
    static void mouse_position_callback(GLFWwindow *gl_win, double x, double y);
    static void mouse_scroll_callback(GLFWwindow *gl_win, double x_off, double y_off);
    static void path_drop_callback(GLFWwindow* gl_win, int count, const char** paths);

    GLFWwindow* window;
};

} /* namespace graphics*/

#endif //GRAPHICS_WINDOW_H
