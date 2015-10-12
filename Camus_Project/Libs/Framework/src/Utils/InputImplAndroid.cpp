#include <Utils/InputBase.h>

namespace hyperspace {

	void EventManager::InitKeyboard(int num_keys) {
		devices[hyperspace::TypeInput_::KEYBOARD].typeInput = hyperspace::TypeInput_::KEYBOARD;
	}

	void EventManager::InitMouse(int num_buttons) {
		devices[hyperspace::TypeInput_::MOUSE].typeInput = hyperspace::TypeInput_::MOUSE;
	}

	void EventManager::InitTouchScreen(int num_touchPoints) {
		devices[hyperspace::TypeInput_::TOUCH_SCREEN].typeInput = hyperspace::TypeInput_::TOUCH_SCREEN;
		for (int i = 0; i < num_touchPoints; i++) {
			hyperspace::InputButton_ tmp_button;
			devices[hyperspace::TypeInput_::TOUCH_SCREEN].buttons.push_back(tmp_button);
		}
	}

	void EventManager::InitAccelerometer() {
		devices[hyperspace::TypeInput_::ACCELEROMETER].typeInput = hyperspace::TypeInput_::ACCELEROMETER;
	}

	void EventManager::InitGyroscope() {
		devices[hyperspace::TypeInput_::GYROSCOPE].typeInput = hyperspace::TypeInput_::GYROSCOPE;
	}

}