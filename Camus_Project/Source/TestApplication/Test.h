#pragma once

#include <Core/Core.h>

class TestApp : public hyperspace::AppBase {
public:
	TestApp() : hyperspace::AppBase() {}
	void CreateAssets();
	void DestroyAssets();

	void OnUpdate(unsigned int dt);
	void OnDraw();
	void OnInput();

	void OnPause();
	void OnResume();

	void OnReset();
};