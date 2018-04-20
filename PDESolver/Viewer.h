/*
    Created by Alejandro Daniel Noel on 29/05/2017.
*/

#ifndef SIMULATOR_VIEWER_H
#define SIMULATOR_VIEWER_H

#include <iostream>
#include "graphics/Window.h"
#include "graphics/ResourceManager.h"
#include "Region.h"
namespace PDE {
class Viewer : public graphics::View {
public:
    void setGrid(Region &baseRegion);
    void render();
};

} /*Namespace PDE*/

#endif //SIMULATOR_VIEWER_H
