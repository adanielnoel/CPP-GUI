/*
    Created by Alejandro Daniel Noel on 02/12/2016.
*/

#include <iostream>
#include "graphics/Window.h"
#include "graphics/ResourceManager.h"


int main() {
    int width = 800;
    int height = 500;
    graphics::resourceManager.init();
    graphics::Window window(width, height, "Testing window 2");
    graphics::resourceManager.addWindow(window);

}