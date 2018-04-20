/*
    Created by Alejandro Daniel Noel on 26/11/2016.
*/

#ifndef GRAPHICS_VIEW_H
#define GRAPHICS_VIEW_H

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <algorithm>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.h"
#include "Event.h"
#include "GeometryPrimitive.h"
#include "Primitives.h"

namespace graphics {

class Window;   // From Window.h

class View {
public:
    View();

    int z = 1000; ///< depth of the view. Used to decide what view is in front of another

    /**
     * Sets parent view. This method automatically sets the container window to that of the parent view.
     * It also sets itself as child of the parent view. `z` is set +1 from its parent.
     * @param superView pointer to parent view.
     */
    void setParent(View &superView);

    /**
     * Sets parent window. Used for a window to initialise its container view.
     * @param window pointer to parent window.
     */
    void setWindow(Window &window);

    /**
     *
     * @return pointer to container window
     */
    Window* getWindow();

    /**
     *
     * @return view's containing rectangle within the coordinate system of the parent view
     */
    Rect getRect();

    //@{ Starts method group
    /**
     * Set view's containing rectangle within the coordinate system of the parent view
     */
    void setRect(Rect rect);
    void setRect(float x, float y, float w, float h);
    void setOrigin(float x, float y);
    void setSize(float w, float h);
    //@} Ends method group

    /**
     * Indicates that the view should render its contents from scratch, so use only when the view needs to be refreshed.
     * This propagates to the parent views, which will also render.
     */
    void setNeedsRender();

    /**
     * First propagates events to child views. If the event has not been handled, handling is attempted.
     * If the view can't handle the event, it discards it and the parent view will attempt to handle it.
     * Note that mouse events that fall outside the view are directly discarded, not passing it to subviews.
     * @param event
     */
    void dispatchEvent(Event *event);

    /**
     * Virtual method to implement by subclasses of View.
     * @return briew description that is used for debugging and logging purposes.
     */
    virtual std::string description() { return "Generic containerView";}

    /**
     *
     * @param coordinates in the view's local refference system
     * @return coordinates in the container window's refference system
     */
    glm::fvec2 localToGlobal(glm::fvec2 coordinates);

    /**
     *
     * @param coordinates in the container window's reference system
     * @return coordinates in the view's local reference system
     */
    glm::fvec2 globalToLocal(glm::fvec2 coordinates);

    /**
     *
     * @return the view's rect in the window's reference system
     */
    Rect globalRect();

    /**
     * Recursively queries the subviews of subviews.
     * @return a vector containing all views trailing the correspondent view tree branch
     */
    std::vector<View*> getChildrenFlatTree();

    /**
     * Draws the current view onto the window.
     * This method is automatically called by the containing window and does not need to be used outside there.
     */
    void draw(bool renderOwn=true);

    /**
     *
     */
     GLuint getTextureBuffer();

protected:
    /**
     * Implement subclassed view rendering here.
     */
    virtual void render();

    /**
     * Implement subclassed view event handling here.
     */
    virtual void handleEvent(Event *event) {};
    View *superView;
    std::vector<View*> subViews;
    Window *window;
    Rect rect;

private:
    void addChild(View &subView);

    /**
     * Sets up buffers for rendering onto the view's texture. Also creates the buffers if they haven't been created yet
     * (if they hold the value -1 or, equivalently, the maximum of GLuint). To be called for buffer creation, resizing
     * or change in number of samples.
     * @param sampleCount number of sampled for antialiasing. Set to 0 for regular buffers.
     * @param width
     * @param height
     */
    void setupOffscreenBuffers(GLsizei sampleCount,
                                      GLsizei width, GLsizei height);
    bool needsRender = true;
    bool didResize;


    /**
     * Index of the frame buffer to which the texture buffer is attached.
     * Bind it by calling `glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);` before drawing code.
     * An index of -1 indicates that the buffer is not yet created.
     */
    GLuint frameBuffer = GLuint(-1);
    GLuint frameBufferMultisample = GLuint(-1);

    /**
     * Texture buffer used as color attachment for the frameBuffer
     * View is rendered onto a texture that the is displayed onto a rect.
     * The reason for this is that animations can then be applied when drawing the textured rect (much easier).
     * `textureBuffer` stores the index of this texture, which resides in the VRAM and is managed by OpenGL.
     */
    GLuint textureBuffer = GLuint(-1);
    GLuint colorRenderBufferMultisample = GLuint (-1);

    /**
     * Render buffer object used as depth attachment for the frame buffer.
     */
    GLuint depthRenderBuffer = GLuint(-1);
    GLuint depthRenderBufferMultisample = GLuint(-1);

    /**
     * Flag that indicates wether all the buffers are already allocated in OpenGL
     */
    bool buffersAreAllocated = false;
};

} /* namespace graphics*/

#endif //GRAPHICS_VIEW_H
