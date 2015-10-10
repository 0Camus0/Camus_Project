#include <Utils/InputBase.h>

void EventManager::InitKeyboard(int num_keys) {
	devices[CamusSpace::TypeInput_::KEYBOARD].typeInput = CamusSpace::TypeInput_::KEYBOARD;
}

void EventManager::InitMouse(int num_buttons) {
	devices[CamusSpace::TypeInput_::MOUSE].typeInput = CamusSpace::TypeInput_::MOUSE;
}

void EventManager::InitTouchScreen(int num_touchPoints) {
	devices[CamusSpace::TypeInput_::TOUCH_SCREEN].typeInput = CamusSpace::TypeInput_::TOUCH_SCREEN;
	for (int i = 0; i < num_touchPoints ; i++){
		CamusSpace::InputButton_ tmp_button;
		devices[CamusSpace::TypeInput_::TOUCH_SCREEN].buttons.push_back(tmp_button);
	}	
}

void EventManager::InitAccelerometer() {
	devices[CamusSpace::TypeInput_::ACCELEROMETER].typeInput = CamusSpace::TypeInput_::ACCELEROMETER;
}

void EventManager::InitGyroscope() {
	devices[CamusSpace::TypeInput_::GYROSCOPE].typeInput = CamusSpace::TypeInput_::GYROSCOPE;
}
template<typename T>
void EventManager::FillTouchCoords(T x, T y, int id) {
	devices[CamusSpace::TypeInput_::TOUCH_SCREEN].currentcoords[id].factual_coords[0] = x;
	devices[CamusSpace::TypeInput_::TOUCH_SCREEN].currentcoords[id].factual_coords[1] = y;
}
