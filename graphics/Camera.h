/*
    Created by Alejandro Daniel Noel on 15/01/2017.
*/

#ifndef SIMULATOR_CAMERA_H
#define SIMULATOR_CAMERA_H

#include "Event.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace graphics {

class Camera {
private:
    glm::mat4 view_matrix;

public:
    void handleEvent(Event &theEvent);
};

} /* namespace graphics*/
#endif //SIMULATOR_CAMERA_H
