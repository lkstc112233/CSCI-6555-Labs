#ifndef INTERFACE_MOUSEHANDLER_H
#define INTERFACE_MOUSEHANDLER_H

#include <functional>

enum MouseConstants
{
    MOUSE_LEFTBUTTON_PRESSED = 1,
    MOUSE_RIGHTBUTTON_PRESSED = 2,
    MOUSE_SCROLLED_UP_PRESSED = 4,
    MOUSE_SCROLLED_DOWN_PRESSED = 8,
};

class MouseHandler
{
  private:
    std::function<void(int mouseConstants, float x, float y)> handleEvent;

  public:
};

#endif // INTERFACE_MOUSEHANDLER_H