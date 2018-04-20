/*
    Created by Alejandro Daniel Noel on 29/12/2016.
*/

#ifndef SIMULATOR_EVENT_H
#define SIMULATOR_EVENT_H

#include <vector>
#include <string>

namespace graphics {

enum eventTypes {
    EVENT_GENERIC,
    EVENT_MOUSE_MOVE,
    EVENT_MOUSE_BUTTON,
    EVENT_MOUSE_SCROLL,
    EVENT_KEY,
    EVENT_CHARACTER,
    EVENT_PATH_DROP
};


class Event {
protected:
    bool dispatched = false;
public:
    virtual bool isDispatched() { return dispatched;}
    virtual void setDispatched() { dispatched = true;}
    inline virtual int type() { return EVENT_GENERIC;}
    inline virtual std::string description() { return "Generic event";}
};

class MouseButtonEvent : public Event {
private:
    int x, y;
    int button;
    int action;
    int mods;
public:
    MouseButtonEvent(int x, int y, int button, int action, int mods) :
            x(x), y(y), button(button), action(action), mods(mods) {}
    int getX() { return x;}
    int getY() { return y;}
    int getButton() { return button;}
    int getAction() { return action;}
    int getmods() { return mods;}
    inline int type() { return EVENT_MOUSE_BUTTON;}
    inline std::string description() { return "Mouse button event";}
};

class MouseMoveEvent : public Event {
private:
    int x, y;
public:
    MouseMoveEvent(int x, int y) : x(x), y(y) {};
    int getX() { return x;}
    int getY() { return y;}
    inline int type() { return EVENT_MOUSE_MOVE;}
    inline std::string description() { return "Mouse move event";}
};

class MouseScrollEvent : public Event {
private:
    int x, y;
    double x_off, y_off;
public:
    MouseScrollEvent(int x, int y, double x_off, double y_off) : x(x), y(y), x_off(x_off), y_off(y_off) {}
    int getX() { return x;}
    int getY() { return y;}
    double getX_off() { return x_off;}
    double getY_off() { return y_off;}
    inline int type() { return EVENT_MOUSE_SCROLL;}
    inline std::string description() { return "Mouse scroll event";}
};

class KeyEvent : public Event {
private:
    int key;
    int scancode;
    int action;
    int mode;
public:
    KeyEvent(int key, int scancode, int action, int mode) :
            key(key), scancode(scancode), action(action), mode(mode) {}
    int getKey() { return key;}
    int getScancode() { return scancode;}
    int gteAction() { return action;}
    int getMode() { return mode;}
    inline int type() { return EVENT_KEY;}
    inline std::string description() { return "Key event";}
};

class CharacterEvent : public Event {
private:
    unsigned int UTF_char;
public:
    CharacterEvent(unsigned int UTF_char) : UTF_char(UTF_char) {}
    int getUTF_char() { return UTF_char;}
    inline int type() { return EVENT_CHARACTER;}
    inline std::string description() { return "Character event";}
};

class PathDropEvent : public Event {
private:
    std::vector<std::string> paths;
public:
    PathDropEvent(std::vector<std::string> paths) : paths(paths) {}
    std::vector<std::string> getPaths() { return paths;}
    inline int type() { return EVENT_PATH_DROP;}
    inline std::string description() { return "Path drop event";}
};

} /* namespace graphics*/

#endif //SIMULATOR_EVENT_H
