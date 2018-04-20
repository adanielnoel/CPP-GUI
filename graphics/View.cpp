/*
    Created by Alejandro Daniel Noel on 26/11/2016.
*/

#include "View.h"
#include "Window.h"
#include <random>
#include "Text.h"
#include "ResourceManager.h"

namespace graphics {

void View::addChild(View &subView) {
    // Add only if not already added
    if(std::find(subViews.begin(), subViews.end(), &subView) == subViews.end())
        this->subViews.push_back(&subView);
}


void View::setParent(View &superView) {
    this->superView = &superView;
    window = superView.window;
    superView.addChild(*this);
    // FIXME: This is used for testing
    z = superView.z + 100;
}

Window *View::getWindow() {
    return window;
}

Rect View::getRect() {
    return rect;
}

void View::setRect(Rect rect) {
    this->rect = rect;
    didResize = true;
}

void View::setRect(float x, float y, float w, float h) {
    this->rect = Rect(x, y, w, h);
    didResize = true;
}

void View::setOrigin(float x, float y) {
    this->rect.x = x;
    this->rect.y = y;
    window->setNeedsRedraw();
}

void View::setSize(float w, float h) {
    this->rect.w = w;
    this->rect.h = h;
    didResize = true;
}

void View::setNeedsRender() {
    needsRender = true;
    window->setNeedsRedraw();
    if (superView) superView->setNeedsRender();
}

void View::draw(bool renderOwn) {
//    std::cout << "Called draw()" << std::endl;
    for (auto view : subViews) {
        view->draw();
    }

    if (needsRender){

        if (!buffersAreAllocated || didResize) {
            setupOffscreenBuffers(4,
                                  GLsizei(rect.w * window->getDPIScaling()),
                                  GLsizei(rect.h * window->getDPIScaling()));
            didResize = false;
        }

        resourceManager.current_projection_matrix = glm::ortho(0.0f, rect.w, rect.h, 0.0f);

        // Setup render to texture
        // The multisampled texture contains different rendering layers that are later used for antialiasing
        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferMultisample);
        glViewport(0, 0, GLsizei(rect.w * window->getDPIScaling()), GLsizei(rect.h * window->getDPIScaling()));
         glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer now

        // Run containerView's OpenGL drawing commands
        if (renderOwn) render();

        // Render children views' textures
        resourceManager.useShader(TEXTURE_2D_SHADER);
        for (auto view : subViews) {
            resourceManager.shader(TEXTURE_2D_SHADER).sendZ(view->z);
            drawTexture(view->getRect(), view->textureBuffer);
        }

        // Draw the multisampled texture onto a regular texture so that it can be used by the texture shader
        glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBufferMultisample);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
        glBlitFramebuffer(0,0, GLsizei(rect.w * window->getDPIScaling()), GLsizei(rect.h * window->getDPIScaling()),
                          0,0, GLsizei(rect.w * window->getDPIScaling()), GLsizei(rect.h * window->getDPIScaling()),
                          GL_COLOR_BUFFER_BIT, GL_NEAREST);

        needsRender = false;
    }
}


void View::render() {
    resourceManager.useShader(COLOR_2D_SHADER);
//    Rectangle::draw(Rect(100.0, 100.0, 100, 100), 1.0, 0.0, 0.0, 1.0);
    Text message("Testing window");
    resourceManager.useShader(TEXT_2D_SHADER);
    message.put(80, 13, 13, glm::vec3(0.0, 0.0, 0.0));
    drawRectangle(Rect(20, 10, 20, 20));
    RoundedRectangle rectRound(Rect(20, 50, 80, 80), 40.f);
    rectRound.setFillColor(0.9, 0.5, 0.5);
    rectRound.draw();
    drawLine({10, 10}, {40, 100}, 0.3);
}

std::vector<View *> View::getChildrenFlatTree() {
    if (subViews.empty()) return std::vector<View *>();
    else {
        std::vector<View *> ft;
        for(auto subView: subViews){
            std::vector<View *> ftc = subView->getChildrenFlatTree();
            ft.insert(ft.begin(), std::make_move_iterator(ftc.begin()), std::make_move_iterator(ftc.end()));
        }
        return ft;
    }
}

// Do not override. Instead, use handleEvent(Event &event) method!
void View::dispatchEvent(Event *event) {
    // If the event comes from the mouse and the containerView does not contain it, then ignore it for the rest of the containerView hierarchy
    if (event->type() == EVENT_MOUSE_SCROLL || event->type() == EVENT_MOUSE_MOVE || event->type() == EVENT_MOUSE_BUTTON){
        // MouseMoveEvent, MouseScrollEvent and MouseButtonEvent all contain getX() and getY(),
        // so casting for any of them will provide these methods
        if (!rect.containsCoords(static_cast<MouseMoveEvent*>(event)->getX(),
                                 static_cast<MouseMoveEvent*>(event)->getY())) {
            return;
        }
    }

    // Propagate the event through the hierarchy
    for (auto view : subViews) {
        view->dispatchEvent(event);
    }

    // Ignore the event if it hasn't been handled, it comes from the keyboard and this containerView is not the first responder
    if (event->type() == EVENT_CHARACTER || event->type() == EVENT_KEY) {
        if (this != window->firstResponder) return;
    }

    // Try to handle the event
    if (!event->isDispatched()) handleEvent(event);

    // This containerView and its forward hierarchy couldn't handle the event
    if (event->isDispatched()) std::cout << "Event dispatched by " << description() << std::endl;
}

glm::fvec2 View::localToGlobal(glm::fvec2 coordinates) {
    if (superView == NULL) return coordinates + rect.origin();
    else return superView->localToGlobal(coordinates + rect.origin());
}

glm::fvec2 View::globalToLocal(glm::fvec2 coordinates) {
    return coordinates - localToGlobal(rect.origin());
}

Rect View::globalRect() {;
    Rect transposedRect = rect.clone();
    transposedRect.transpose(localToGlobal(-rect.origin()));
    return transposedRect;
}

GLuint View::getTextureBuffer() {
    return textureBuffer;
}

void View::setupOffscreenBuffers(GLsizei sampleCount, GLsizei width, GLsizei height) {
    // Create buffers if they have not been created yet
    if (frameBuffer == -1)   glGenFramebuffers(1, &frameBuffer);
    if (textureBuffer == -1) glGenTextures(1, &textureBuffer);
    if (depthRenderBuffer == -1)   glGenRenderbuffers(1, &depthRenderBuffer);
    if (frameBufferMultisample == -1)   glGenFramebuffers(1, &frameBufferMultisample);
    if (depthRenderBufferMultisample == -1)   glGenRenderbuffers(1, &depthRenderBufferMultisample);
    if (colorRenderBufferMultisample == -1) glGenRenderbuffers(1, &colorRenderBufferMultisample);

    // Configure a multisampled framebuffer object
    // NOTE: All attachment images must have the same # of samples.
    // Otherwise, the framebuffer status will not be complete.
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferMultisample);

        // Configure a multisampled renderbuffer object to store color info
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBufferMultisample);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, sampleCount, GL_RGBA, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // Configure a multisampled renderbuffer object to store depth info
        // NOTE: A depth renderable image should be attached the FBO for depth test.
        // If we don't attach a depth renderable image to the FBO, then
        // the rendering output will be corrupted because of missing depth test.
        // If you also need stencil test for your rendering, then you must
        // attach additional image to the stencil attachment point, too.
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBufferMultisample);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, sampleCount, GL_DEPTH_COMPONENT, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // Attach multisampled RBOs to FBO attachment points
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBufferMultisample);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBufferMultisample);

        // Check if the (multisampled) framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    // Configure a normal (no multisampling) FBO to render the multisampled fbo to the containerView's texture
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glBindTexture(GL_TEXTURE_2D, textureBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        // attach a texture to FBO color attachment point
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer, 0);

        // attach a rbo to FBO depth attachment point
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

        // Check if the framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    buffersAreAllocated = true;
}

View::View() {

}

void View::setWindow(Window &window) {
    this->window = &window;
}

} /* namespace graphics*/
