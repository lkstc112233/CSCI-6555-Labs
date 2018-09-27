#ifndef INTERFACE_MOUSEHANDLER_H
#define INTERFACE_MOUSEHANDLER_H

#include <vector>
#include <functional>

enum MouseConstants
{
	MOUSE_LEFTBUTTON_PRESSED = 1,
	MOUSE_RIGHTBUTTON_PRESSED = 2,
	MOUSE_LEFTBUTTON_HOLD = 4,
	MOUSE_RIGHTBUTTON_HOLD = 8,
    MOUSE_SCROLLED_UP_PRESSED = 16,
    MOUSE_SCROLLED_DOWN_PRESSED = 32,
};

/**
 * A class managing mouse operations.
 */
class MouseHandler
{
  private:
	  std::function<void(int mouseFlags, float clampedx, float clampedy, float diffx, float diffy)> handleEvent;
	  friend class MouseHandlerContainer;
  public:
	  MouseHandler(std::function<void(int mouseFlags, float clampedx, float clampedy, float diffx, float diffy)> handleEvent);
	  MouseHandler(std::function<void(int mouseFlags, float clampedx, float clampedy)> handleEvent);
	  MouseHandler(std::function<void(int mouseFlags)> handleEvent);
};

class MouseHandlerContainer {
  private:
	std::vector<MouseHandler> handlers;
	int flags = 0;
	float xClamp;
	float yClamp;
	float x;
	float y;
	float diffx;
	float diffy;
	float clampedx;
	float clampedy;

  public:
	MouseHandlerContainer(float xInit, float yInit, float xClamp, float yClamp);
	void setScrollUpFlag();
	void setScrollDownFlag();
	void leftPress();
	void rightPress();
	void resetLeftHoldFlag();
	void resetRightHoldFlag();
	void setPosition(float x, float y);
	void handle();
	template <class... Args>
	void emplace_handler(Args &&... args)
	{
		handlers.emplace_back(std::forward<Args>(args)...);
	}
};
#endif // INTERFACE_MOUSEHANDLER_H