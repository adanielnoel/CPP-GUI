/*
    Created by Alejandro Daniel Noel on 23/03/2017.
*/

#ifndef SIMULATOR_PRIMITIVES_H
#define SIMULATOR_PRIMITIVES_H

#include "glm/glm.hpp"

namespace graphics {
struct Rect {
    float x; ///< origin x-coordinate (top-left corner)
    float y; ///< origin y-coordinate (top-left corner)
    float w; ///< width
    float h; ///< height
    /**
     * Rectangle constructor
     * @param x coordinate of the origin
     * @param y coordinate of the origin
     * @param w width of the rectangle
     * @param h height of the rectangle
     */
    Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}

    /**
     * Default rectangle constructor. Initialises all dimensions to 0.
     */
    Rect() : x(0), y(0), w(0), h(0) {}

    /**
     * Copy constructor
     * @param rect to copy
     */
    inline Rect clone() const { return *(new Rect(*this)); }

    //@{ Starts method group
    /**
     * @return true if the coordinates are inside the rectangle
     */
    inline bool containsCoords(float x, float y) {
        if ((x > this->x && y > this->y) && (x < this->x + this->w && y < this->y + this->h)) return true;
        else return false;
    }
    //@} Ends method group

    /**
     *
     * @return coordinates of the origin
     */
    inline glm::fvec2 origin() { return glm::fvec2(x, y); }

    inline Rect transpose(float xOff, float yOff) {
        x += xOff;
        y += yOff;
        return *this;
    }

    inline Rect transpose(glm::fvec2 shift) {
        return transpose(shift.x, shift.y);
    }
};


struct Size {
    float w;
    float h;

    Size(float w, float h) : w(w), h(h) {}

    Size() : w(0), h(0) {}
};

struct Color {
    float r, g, b, a;

    Color(float r, float g, float b, float a = 1.f) : r(r), g(g), b(b), a(a) {}
    Color() : Color(0.f, 0.f, 0.f, 0.f) {};

    inline glm::vec4 getVector() { return glm::vec4(r, g, b, a); }
};

} /* namespace graphics*/
#endif //SIMULATOR_PRIMITIVES_H
