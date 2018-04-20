/*
    Created by Alejandro Daniel Noel on 16/12/2016.
*/

#include "View3D.h"
#include "Window.h"
#include "ResourceManager.h"

namespace graphics {

void View3D::render() {
    resourceManager.shader(COLOR_3D_SHADER).sendProjectionMatrix(glm::perspective(45.0f, rect.w/rect.h, 0.1f, 100.0f));
    resourceManager.shader(COLOR_3D_SHADER).use();
    // TODO: A memory leak seems to be occurring inside this function
    baseElement->render();
}

View3D::View3D(graphics::Element &baseElement) {
    this->baseElement = &baseElement;
    baseElement.setContainerView(*this);
}

std::string View3D::description() {
    return "generic 3D containerView";
}

} /* namespace graphics*/
