/*
 * Created by Alejandro Daniel Noel on 27/11/2016.
 *
 */

#include "Window.h"
#include "Element.h"
#include "View3D.h"
#include "ResourceManager.h"

using namespace graphics;

int main () {
    resourceManager.init();
//    std::cout << ResourceManager::colorForFeature("background").r << std::endl;
//    return 1;
    int width = 1600, height = 400;
//    Window window(width, height, "Testing window");
//    app.addWindow(window);
    Window window2(800, 400, "Testing window 2");
    resourceManager.addWindow(window2);

    std::vector<glm::vec3> vertices = {(glm::vec3) {-0.5f, -0.5f, -0.5f},
                                       (glm::vec3) {0.5f,  -0.5f, -0.5f},
                                       (glm::vec3) {0.5f,   0.5f, -0.5f},
                                       (glm::vec3) {0.5f,   0.5f, -0.5f},
                                       (glm::vec3) {-0.5f,  0.5f, -0.5f},
                                       (glm::vec3) {-0.5f, -0.5f, -0.5f},
                                       (glm::vec3) {-0.5f, -0.5f,  0.5f},
                                       (glm::vec3) {0.5f,  -0.5f,  0.5f},
                                       (glm::vec3) {0.5f,   0.5f,  0.5f},
                                       (glm::vec3) {0.5f,   0.5f,  0.5f},
                                       (glm::vec3) {-0.5f,  0.5f,  0.5f},
                                       (glm::vec3) {-0.5f, -0.5f,  0.5f},
                                       (glm::vec3) {-0.5f,  0.5f,  0.5f},
                                       (glm::vec3) {-0.5f,  0.5f, -0.5f},
                                       (glm::vec3) {-0.5f, -0.5f, -0.5f},
                                       (glm::vec3) {-0.5f, -0.5f, -0.5f},
                                       (glm::vec3) {-0.5f, -0.5f,  0.5f},
                                       (glm::vec3) {-0.5f,  0.5f,  0.5f},
                                       (glm::vec3) {0.5f,   0.5f,  0.5f},
                                       (glm::vec3) {0.5f,   0.5f, -0.5f},
                                       (glm::vec3) {0.5f,  -0.5f, -0.5f},
                                       (glm::vec3) {0.5f,  -0.5f, -0.5f},
                                       (glm::vec3) {0.5f,  -0.5f,  0.5f},
                                       (glm::vec3) {0.5f,   0.5f,  0.5f},
                                       (glm::vec3) {-0.5f, -0.5f, -0.5f},
                                       (glm::vec3) {0.5f,  -0.5f, -0.5f},
                                       (glm::vec3) {0.5f,  -0.5f,  0.5f},
                                       (glm::vec3) {0.5f,  -0.5f,  0.5f},
                                       (glm::vec3) {-0.5f, -0.5f,  0.5f},
                                       (glm::vec3) {-0.5f, -0.5f, -0.5f},
                                       (glm::vec3) {-0.5f,  0.5f, -0.5f},
                                       (glm::vec3) {0.5f,   0.5f, -0.5f},
                                       (glm::vec3) {0.5f,   0.5f,  0.5f},
                                       (glm::vec3) {0.5f,   0.5f,  0.5f},
                                       (glm::vec3) {-0.5f,  0.5f,  0.5f},
                                       (glm::vec3) {-0.5f,  0.5f, -0.5f}};

    Element elem0(vertices);
    Element elem1(vertices);
    elem1.setParent(elem0);
    elem1.translate(2.0, 0.0, 0.0);

    View3D view3D(elem0);
    view3D.setParent(window2.getView());
    view3D.setRect(400, 0, 400, 400);

    View testView1;
    testView1.setRect(0, 0, 200, 200);
    testView1.setParent(window2.getView());
    testView1.z = 100;
    view3D.z = 50;

    resourceManager.loadColorScheme(COLOR_SCHEME_DARK);

    float angle = 1.0;
    while (resourceManager.running()){
        std::cout << "----" << window2.getFrameCount() << std::endl;
        elem0.rotateY(glm::radians(angle));
        elem1.rotateY(glm::radians(5 * angle));
        view3D.setNeedsRender();
//        testView1.setNeedsRender();
        resourceManager.cycle();
        // TODO: Move main loop completely into App
    }

    glfwTerminate();
    return 0;

}
