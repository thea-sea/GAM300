#include "Input.hxx"
#include "Rendering/GraphicsManagerWrapperCalls.h"
#include "Windows.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace ScriptAPI
{
	void Input::InputSetup()
	{
		AxisDictionary = gcnew System::Collections::Generic::Dictionary<System::String^, System::Tuple<uint32_t, uint32_t>^>();
		AxisDictionary->Add("Horizontal", gcnew System::Tuple<uint32_t, uint32_t>(TDS_D, TDS_A));
		AxisDictionary->Add("Vertical", gcnew System::Tuple<uint32_t, uint32_t>(TDS_W, TDS_S));
		AxisDictionary->Add("Mouse X", gcnew System::Tuple<uint32_t, uint32_t>(Keycode::M1, Keycode::M1));
		AxisDictionary->Add("Mouse Y", gcnew System::Tuple<uint32_t, uint32_t>(Keycode::M2, Keycode::M2));
	}

	float Input::GetAxisRaw(System::String^ code)
	{
		if (GetKeyDown(AxisDictionary[code]->Item1))
		{
			return 1.0f;
		}
		if (GetKeyDown(AxisDictionary[code]->Item2))
		{
			return -1.0f;
		}

		return 0.0f;
	}

	float Input::GetAxis(System::String^ code)
	{
		float AxisValue = 0.0f;
		float minValue = -1.0f;
		float maxValue = 1.0f;
		//std::cout << mousePositionX << "\n";
		if (AxisDictionary[code]->Item1 >= TDS_MAX_KEYS)
		{
			if (code == "Mouse X") // X (may want to change)
			{
				float getMouseDifferenceX = GetMousePositionDifferenceX();
				//std::cout << GetMousePositionDifferenceX()<<"\n";
				AxisValue += getMouseDifferenceX * 0.01;
			}
			else if (code == "Mouse Y")// Y (may want to change)
			{
				float getMouseDifferenceY = GetMousePositionDifferenceY();
				AxisValue -= getMouseDifferenceY * 0.01;
			}
		}
		else
		{
			if (GetKeyDown(AxisDictionary[code]->Item1))
			{
				AxisValue += 1.0f;
			}
			if (GetKeyDown(AxisDictionary[code]->Item2))
			{
				AxisValue -= 1.0f;
			}
		}

		return std::clamp<float>(AxisValue, minValue, maxValue);
	}

	bool Input::GetKey(uint32_t keycode)
	{
		return TDS::Input::isKeyPressed(keycode);
	}

	bool Input::GetKeyDown(uint32_t keycode)
	{
		return TDS::Input::isKeyPressed(keycode);
	}

	bool Input::GetKeyUp(uint32_t keycode)
	{
		return TDS::Input::isKeyReleased(keycode);
	}

	void Input::KeyRelease(uint32_t keycode)
	{
		return TDS::Input::KeyRelease(keycode);
	}

	bool Input::GetMouseButton(uint32_t button)
	{
		return TDS::Input::isMouseButtonPressed(button - Keycode::MAX_KEYS);
	}

	bool Input::GetMouseButtonDown(uint32_t button)
	{
		return TDS::Input::isMouseButtonPressed(button - Keycode::MAX_KEYS);
	}

	bool Input::GetMouseButtonUp(uint32_t button)
	{
		return TDS::Input::isMouseButtonReleased(button - Keycode::MAX_KEYS);
	}

	float Input::GetMousePositionDifferenceX()
	{
		return TDS::Input::getMousePosition().x - mousePositionX;
	}
	float Input::GetMousePositionDifferenceY()
	{
		return mousePositionY - TDS::Input::getMousePosition().y;
	}

	Vector3 Input::GetMousePosition()
	{
		return Vector3(TDS::Input::getMousePosition().x, TDS::Input::getMousePosition().y, 0.0f);
	}

	float Input::GetLocalMousePosX()
	{
		return TDS::Input::getLocalMousePosX();
		//return TDS::Input::final_x_pos;
		//return TDS::Input::GetObjectPickPosX();
	}

	float Input::GetLocalMousePosY()
	{
		return TDS::Input::getLocalMousePosY();
		//return TDS::Input::final_y_pos;
		//return TDS::Input::GetObjectPickPosY();
	}

	// float Input::GetPickedPosX()
	// {
	// 	return TDS::Input::GetObjectPickPosX();
	// }

	// float Input::GetPickedPosY()
	// {
	// 	return TDS::Input::GetObjectPickPosY();
	// }

	void Input::InputUpdate()
	{
		mousePositionX = TDS::Input::getMousePosition().x;
		mousePositionY = TDS::Input::getMousePosition().y;
		int width = TDS::GetScreenWidth();
		int height = TDS::GetScreenHeight();
		if (mousePositionX > 0.75f*width || mousePositionX < 0.25f*width) {
			//CenterMouseX();
		}
		if (mousePositionY > 0.9f * height || mousePositionY < 0.1f * height)
		{
			//CenterMouseY();
		}
		//std::cout << mousePositionX << std::endl;
	}

	void Input::HideMouse() {
		ShowCursor(FALSE);
	}

	void Input::SetMousePosX(uint32_t Xpos) {
		mousePositionX = Xpos;
	}

	void Input::SetMousePosY(uint32_t Ypos) {
		mousePositionY = Ypos;
	}

	void Input::CenterMouseX() {
		POINT currentMousePos;
		GetCursorPos(&currentMousePos);

		int width = TDS::GetScreenWidth();

		//get top left of window
		int windowX = TDS::GraphicsManagerWrapperCalls::GetWinPosX();

		int centerX = windowX + (width / 2);

		//std::cout << mousePositionX << "\n";
		//std::cout << centerX<<"\n";
		SetCursorPos(centerX, currentMousePos.y);
		//mousePositionX = TDS::Input::getMousePosition().x;
		mousePositionX = width / 2;
		//std::cout << currentMousePos.x << "\n" << mousePositionX << "\n";
		//LastMousePosY = height/2;
	}

	void Input::CenterMouseY() {
		POINT currentMousePos;
		GetCursorPos(&currentMousePos);

		int height = TDS::GetScreenHeight();

		//get top left of window
		int windowY = TDS::GraphicsManagerWrapperCalls::GetWinPosY();

		int centerY = windowY + (height / 2);

		//std::cout << mousePositionX << "\n";
		//std::cout << centerX<<"\n";
		SetCursorPos(currentMousePos.x, centerY);
		mousePositionY = height / 2;
		//mousePositionY = TDS::Input::getMousePosition().y;
		//mousePositionY = height / 2;

		//LastMousePosY = height/2;*/

	}

	void Input::UpdateLastMousePos(int winX, int winY) {
		//std::cout << LastMousePosX<<"last X\n";
		LastMousePosX = winX;
		LastMousePosY = winY;
		//std::cout << LastMousePosX<<"updated X\n";
	}

	int Input::GetScreenMouseX() {
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		return cursorPos.x;
	}

	int Input::GetScreenMouseY() {
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		return cursorPos.y;
	}

	void Input::CenterLastMousePos() {
		int width = TDS::GetScreenWidth();
		int height = TDS::GetScreenHeight();

		//get top left of window
		int windowX = TDS::GraphicsManagerWrapperCalls::GetWinPosX();
		int windowY = TDS::GraphicsManagerWrapperCalls::GetWinPosY();

		LastMousePosX = windowX + (width / 2);
		LastMousePosY = windowY + (height / 2);

	}

	int Input::ScreenMousePosdX() {
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		return (GetScreenMouseX() - LastMousePosX);
	}

	int Input::ScreenMousePosdY() {
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		return GetScreenMouseY() - cursorPos.y;
	}


	// NEW INPUT CLASS STUFF
	bool InputSystem::GetKey(uint32_t keycode)
	{
		return TDS::InputSystem::GetInstance()->isKeyDown(keycode);
	}

	bool InputSystem::GetKeyDown(uint32_t keycode)
	{
		return TDS::InputSystem::GetInstance()->isKeyPressed(keycode);
	}

	bool InputSystem::GetKeyUp(uint32_t keycode)
	{
		return TDS::InputSystem::GetInstance()->isKeyReleased(keycode);
	}

	bool InputSystem::GetMouseButton(uint32_t keycode)
	{
		return TDS::InputSystem::GetInstance()->isMouseDown(keycode);
	}

	bool InputSystem::GetMouseButtonDown(uint32_t keycode)
	{
		return TDS::InputSystem::GetInstance()->isMousePressed(keycode);
	}

	bool InputSystem::GetMouseButtonUp(uint32_t keycode)
	{
		return TDS::InputSystem::GetInstance()->isMouseReleased(keycode);
	}

	void InputSystem::HideMouse(bool value)
	{
		return TDS::InputSystem::GetInstance()->setCursorVisible(value);
	}

	void InputSystem::Lock(bool value)
	{
		return TDS::InputSystem::GetInstance()->setMouseLock(value);
	}

	bool InputSystem::isMouseLocked()
	{
		return TDS::InputSystem::GetInstance()->getMouseLock();
	}

	float InputSystem::GetLocalMousePosX()
	{
		return TDS::InputSystem::GetInstance()->getLocalMousePosX();
	}

	float InputSystem::GetLocalMousePoxY()
	{
		return TDS::InputSystem::GetInstance()->getLocalMousePosY();
	}

	float InputSystem::GetMouseDeltaX()
	{
		return TDS::InputSystem::GetInstance()->getMouseDeltaX();
	}

	float InputSystem::GetMouseDeltaY()
	{
		return TDS::InputSystem::GetInstance()->getMouseDeltaY();
	}

	float InputSystem::GetRawMouseAxisX()
	{
		return TDS::InputSystem::GetInstance()->getRawMouseInputX();
	}

	float InputSystem::GetRawMouseAxisY()
	{
		return TDS::InputSystem::GetInstance()->getRawMouseInputY();
	}


}
