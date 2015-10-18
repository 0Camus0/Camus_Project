#include <Utils/InputBase.h>
namespace hyperspace {
	namespace input {
		void EventManager::InitKeyboard(int num_keys) {
			devices[TypeInput_::KEYBOARD].typeInput = TypeInput_::KEYBOARD;
		}

		void EventManager::InitMouse(int num_buttons) {
			devices[TypeInput_::MOUSE].typeInput = TypeInput_::MOUSE;
		}

		void EventManager::InitTouchScreen(int num_touchPoints) {
			devices[TypeInput_::TOUCH_SCREEN].typeInput = TypeInput_::TOUCH_SCREEN;
		}

		void EventManager::InitAccelerometer() {
			devices[TypeInput_::ACCELEROMETER].typeInput = TypeInput_::ACCELEROMETER;
		}

		void EventManager::InitGyroscope() {
			devices[TypeInput_::GYROSCOPE].typeInput = TypeInput_::GYROSCOPE;
		}
	}
}