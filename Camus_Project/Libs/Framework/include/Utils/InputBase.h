#ifndef CA_INPUTBASE_H
#define CA_INPUTBASE_H

namespace CamusSpace {
	
// Type of input
enum TypeInput_ {
	KEYBOARD = 0,
	MOUSE,
	TOUCH_SCREEN,
	ACCELEROMETER,
	GYROSCOPE,
};

enum TypeEvent_ {
	KEY_PRESSED = 0,
	KEY_RELEASED,
	MOUSE_PRESSED,
	MOUSE_MOVED,
	MOUSE_RELEASED,
	TOUCH_PRESSED,
	TOUCH_MOVED,
	TOUCH_RELEASED,
	ANALOG_AXIS_X,
	ANALOG_AXIS_Y,
	ANALOG_TRIGGER_L,
	ANALOG_TRIGGER_R,
	JOY_BUTTON_PRESSED,
	JOY_BUTTON_RESEASED,
	SCREEN_RESIZE,
	SCREEN_ON_FOCUS,
	SCREEN_OUT_FOCUS,
};

enum MouseButtons_{
	MOUSE_BUTTON_LEFT = 0,
	MOUSE_BUTTON_RIGHT,
	MOUSE_BUTTON_MIDDLE,
	MOUSE_BUTTON_SCROLL_UP,
	MOUSE_BUTTON_SCROLL_DOWN,
	MOUSE_BUTTON_OPT_1,
	MOUSE_BUTTON_OPT_2,
	MOUSE_BUTTON_OPT_3,
	MOUSE_BUTTON_OPT_4,
	MOUSE_BUTTON_OPT_5,
};

struct InputEvent {
	TypeInput_	typeInput;
	TypeEvent_	typeEvent;
	int			Id;
	union {
		int			iCoords[4];
		float		fCoords[4];
	};
};

struct InputDevice {
	TypeInput_	type;

	
};


}

#include <vector>

class EventManager {
public:
	EventManager(){}

	std::vector<CamusSpace::InputEvent>		m_InputQueue;

};

#endif