#ifndef CA_INPUTBASE_H
#define CA_INPUTBASE_H

#include <vector>

namespace CamusSpace {
	
// Type of input
enum TypeInput_ {
	KEYBOARD = 0,
	MOUSE,
	TOUCH_SCREEN,
	ACCELEROMETER,
	GYROSCOPE,
	JOY_PAD_0,
	JOY_PAD_1,
	JOY_PAD_2,
	JOY_PAD_3,
	STEERING_WHEEL,
	POSITION_TRACKING,
	UNKOWN_DEVICE,
	NUM_DEVICES
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
	UNKOWN_EVENT
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

// 32 bit nice structure
struct InputEvent_ {
	InputEvent_() : _typeInput(UNKOWN_DEVICE), _typeEvent(UNKOWN_EVENT), _id(0), _state(0) { icoords[0] = icoords[1] = icoords[2] = icoords[3] = 0; }
	TypeInput_	_typeInput;
	TypeEvent_	_typeEvent;
	int			_id;
	int			_state;
	union {
		int			icoords[4];
		float		fcoords[4];
	};
};

struct InputButton_ {
	enum { _ON = 1, _OFF = 0 };
	enum { _LOCKED = 1, _UNLOCKED = 0 };
	InputButton_():_state(_OFF) , _lock(_OFF){}
	short	_state;
	short	_lock;
	void	PressIt() { _state = (_LOCKED==1) ? _OFF : _ON; }
	void	ReleaseIt() { _state = _OFF; _lock = _UNLOCKED; }
	void	LockIt() { _lock = _LOCKED; }
	void	UnlockIt() { _lock = _UNLOCKED; }
	bool	isPressed() { return (_state == _ON) ? true : false; }
	bool	isLocked() { return (_lock == _LOCKED) ? true : false; }
};

struct InputDevice_ {
	TypeInput_					typeInput;
	std::vector<InputButton_>	buttons;
	union {
		int			iactual_coords[4];
		float		factual_coords[4];
	};
	bool	isInitialized() { (buttons.size() > 0) ? true : false; }
};

}

class EventManager {
public:
	EventManager(){}
	void InitKeyboard(int num_keys);
	void InitMouse(int num_buttons);
	void InitTouchScreen(int num_touchPoints);
	void InitAccelerometer();
	void InitGyroscope();

	// TODO: Implement other input Devices.
	//void InitJoypad...

	std::vector<CamusSpace::InputEvent_>		queue;
	CamusSpace::InputDevice_					devices[CamusSpace::TypeInput_::NUM_DEVICES];

};

#endif