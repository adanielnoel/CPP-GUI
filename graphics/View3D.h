/*
    Created by Alejandro Daniel Noel on 16/12/2016.
*/

#ifndef SIMULATOR_VIEW3D_H
#define SIMULATOR_VIEW3D_H

#include "View.h"
#include "Element.h"

namespace graphics {

class View3D : public View{
public:
    View3D(graphics::Element &baseElement);
    void render();
    std::string description();

private:
    graphics::Element *baseElement;
};

} /* namespace graphics*/

#endif //SIMULATOR_VIEW3D_H
