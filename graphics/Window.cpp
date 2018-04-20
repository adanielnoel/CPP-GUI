/*
    Created by Alejandro Daniel Noel on 26/11/2016.
*/

#include "Window.h"
#include <thread>
#include "ResourceManager.h"

namespace graphics {

void Window::resize_callback(GLFWwindow *gl_win, int width, int height) {
    Window *window = static_cast<Window*>(glfwGetWindowUserPointer(gl_win));
    window->didResize(width, height);
}

void Window::key_callback(GLFWwindow *gl_win, int key, int scancode, int action, int mode) {
    Window *window = static_cast<Window*>(glfwGetWindowUserPointer(gl_win));
    KeyEvent event(key, scancode, action, mode);
    window->dispatchEvent(&event);
}

void Window::character_callback(GLFWwindow *gl_win, unsigned int UTF_char) {
    Window *window = static_cast<Window*>(glfwGetWindowUserPointer(gl_win));
    CharacterEvent event(UTF_char);
    window->dispatchEvent(&event);
    std::cout << UTF_char << std::endl;
}

void Window::mouse_button_callback(GLFWwindow *gl_win, int button, int action, int mods) {
    Window *window = static_cast<Window*>(glfwGetWindowUserPointer(gl_win));
    int x, y;
    window->getMousePos(&x, &y);
    // mods: SHIFT (+1), CTRL (+2), ALT (+4), CMD (+8)
    MouseButtonEvent event(x, y, button, action, mods);
    window->dispatchEvent(&event);
}

void Window::mouse_position_callback(GLFWwindow *gl_win, double x, double y) {
    Window *window = static_cast<Window*>(glfwGetWindowUserPointer(gl_win));
    if (((int)x < window->getSize().w && (int)y < window->getSize().h) && ((int)x > 0 && (int)y > 0)) {
        MouseMoveEvent event((int)x, (int)y);
        window->dispatchEvent(&event);
    }
}

void Window::mouse_scroll_callback(GLFWwindow *gl_win, double x_off, double y_off) {
    Window *window = static_cast<Window*>(glfwGetWindowUserPointer(gl_win));
    int x, y;
    window->getMousePos(&x, &y);
    MouseScrollEvent event(x, y, x_off, y_off);
    window->dispatchEvent(&event);
}

void Window::path_drop_callback(GLFWwindow *gl_win, int count, const char **paths) {
    Window *window = static_cast<Window*>(glfwGetWindowUserPointer(gl_win));
    std::vector<std::string> n_paths;
    for (int i = 0; i < count; i++) {
        n_paths.push_back(std::string(paths[i]));
    }
    PathDropEvent event(n_paths);
    window->dispatchEvent(&event);
}


Window::Window(int width, int height, std::string title) : width(width), height(height), title(title) {
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return;
    }
#ifdef __APPLE__
    // These lines ask OSX to use the latest supported version of OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        return;
    }

    // SETTING CALLBACKS
    glfwSetWindowUserPointer(window, this);
    glfwSetWindowSizeCallback(window, resize_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCharCallback(window, character_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_position_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);
    glfwSetDropCallback(window, path_drop_callback);
    glfwMakeContextCurrent(window);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit ();

    if (!resourceManager.areShadersLoaded())
        resourceManager.loadShaders();

    // Get the scaling factor from display points to pixels. This is important because
    // in retina displays points are used instead of pixels.
    int pix_w, pix_h;
    glfwGetFramebufferSize(window, &pix_w, &pix_h);
    dpi_scaling = (float)pix_w / width;

    // Set coordinate system in pixels
//    glViewport(0, 0, pix_w, pix_h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glOrtho(0, width, width, 0, -1, 1);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();

    // get version info
    const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString (GL_VERSION); // version as a string
    printf ("Renderer: %s\n", renderer);
    printf ("OpenGL version supported %s\n", version);

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LESS); // depth-testing interpreting a smaller value as "closer"

    // Render only outline
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // For transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // For antialiasing (only applies to the window buffers, views's framebuffers need to be explicitly configured)
    glEnable(GL_MULTISAMPLE);
//    glDisable(GL_MULTISAMPLE);

    timeOfLastUpdate = glfwGetTime();
    actual_fps = 0;
    frameCount = 0;
    setFPS(30.0);

//    std::cout << Shader::shader3DColor()->getProgram() << std::endl;
//    std::cout << Shader::shader2DColor()->getProgram() << std::endl;

    // TODO: TEMPORARY. MOVE TO CAMERA OBJECT
    glm::mat4 view;
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    resourceManager.shader(COLOR_3D_SHADER).sendViewMatrix(view);
//    resourceManager.shader(COLOR_3D_SHADER).sendProjectionMatrix(glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f));
//    resourceManager.shader(COLOR_2D_SHADER).sendProjectionMatrix(glm::ortho(0.0f, (GLfloat)width, (GLfloat)height, 0.0f));
//    resourceManager.shader(TEXTURE_2D_SHADER).sendProjectionMatrix(glm::ortho(0.0f, (GLfloat)width, (GLfloat)height, 0.0f));

    this->setNeedsRedraw();
    // FIXME: use pixel coordinates that are display-independent, by including scaling in the projection
    containerView.setRect(0.0, 0.0, width, height);
    containerView.setWindow(*this);
}

void Window::draw() {
    if (needsRedraw) {
        // FIXME: remove the renderOwn argument, which is only used to prevent rendering the test render in the window's root view
        containerView.draw(false);
        // Render to screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width * (GLsizei) getDPIScaling(), height * (GLsizei) getDPIScaling());
//        glm::vec4 clr_c = resourceManager.colorForFeature("background");
//        glClearColor(clr_c.r, clr_c.g, clr_c.b, clr_c.a);
        // Set window background to red for debugging shaders
        // If they work, no red should be visible
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // The current projection matrix is already set correctly by the containerView,
        // so no need to set it again before using the shader
        resourceManager.useShader(TEXTURE_2D_SHADER);

        drawTexture(containerView.getRect(), containerView.getTextureBuffer());

        // Now draw the containerView into the window
        glfwSwapBuffers(window);
    }
}

void Window::update() {
    // TODO: Implement throttling depending on frequency of events and whether needsRedraw or not
    std::this_thread::sleep_for(std::chrono::microseconds(int(1e6 * (timeBetweenUpdates - (glfwGetTime() - timeOfLastUpdate)))));
    timeOfLastUpdate = glfwGetTime();
    updateFPS();
    // FIXME: This needs to be uncommented in order to use multiple windows.
//    glfwMakeContextCurrent(window);
    glfwPollEvents();

    // Handle window closing. The resource manager will take care of the deallocation
    closed = (bool)glfwWindowShouldClose(window);

    if (!closed){
        draw();
        needsRedraw = false;
    }
}

void Window::updateFPS() {
    double elapsedSeconds;
    static double timeOfLastFPSUpdate = glfwGetTime();
    char tmp[128];
    static int lastFrameCount = 0;

    double currentSeconds = glfwGetTime ();
    elapsedSeconds = currentSeconds - timeOfLastFPSUpdate;

    if (elapsedSeconds > 0.25) {
        timeOfLastFPSUpdate = currentSeconds;

        actual_fps = (frameCount - lastFrameCount) / elapsedSeconds;
        sprintf (tmp, "%s @ fps: %.2f", title.c_str(), actual_fps);
        glfwSetWindowTitle (window, tmp);
        lastFrameCount = frameCount;
    }
    frameCount++;
}

Size Window::getSize() {
    return Size(width, height);
}

void Window::setID(int ID) {
    this->ID = ID;
}

void Window::setNeedsRedraw() {
    needsRedraw = true;
}

void Window::dispatchEvent(Event *event) {
    // recursively send event to children views
    containerView.dispatchEvent(event);

    // If no child containerView could handle the event, send it to the application
    if (!event->isDispatched()) {
        resourceManager.dispatchEvent(event);
    }
}

void Window::didResize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
//    glViewport(0, 0, (GLsizei)newWidth, (GLsizei)newWidth);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glMatrixMode(GL_MODELVIEW);
    // TODO: resolve constraints
}

void Window::getMousePos(int *x, int *y) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    *x = static_cast<int>(xpos);
    *y = static_cast<int>(ypos);
}

Window::~Window() {
    glfwDestroyWindow(window);
}

float Window::getDPIScaling() {
    return dpi_scaling;
}

void Window::setFPS(double required_fps) {
    this->required_fps = required_fps;
    timeBetweenUpdates = 1.0 / required_fps;
}

View &Window::getView() {
    return containerView;
}


} /* namespace graphics*/




