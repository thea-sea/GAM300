#pragma once
#include "IncludeFromEngine.hxx"
#include "Vector3.hxx"
#include "../GAM300Engine/Include/Input/Input.h"
//#include "../GAM300Engine/Include/Rendering/GraphicsManager.h"
#include <algorithm>
#include "Vector2.hxx"

#define Mouse_X TDS::Input::getMousePosition()->x
#define Mouse_Y TDS::Input::getMousePosition()->y

namespace ScriptAPI
{
	/*enum class Keycode : uint32_t
	{
		A = TDS_A,
		B = TDS_B,
		C = TDS_C,
		D = TDS_D,
		E = TDS_E,
		F = TDS_F,
		G = TDS_G,
		H = TDS_H,
		I = TDS_I,
		J = TDS_J,
		K = TDS_K,
		L = TDS_L,
		M = TDS_M,
		N = TDS_N,
		O = TDS_O,
		P = TDS_P,
		Q = TDS_Q,
		R = TDS_R,
		S = TDS_S,
		T = TDS_T,
		U = TDS_U,
		V = TDS_V,
		W = TDS_W,
		X = TDS_X,
		Y = TDS_Y,
		Z = TDS_Z,

		UP = TDS_UP,
		DOWN = TDS_DOWN,
		LEFT = TDS_LEFT,
		RIGHT = TDS_RIGHT,

		ZERO = TDS_0,
		ONE = TDS_1,
		TWO = TDS_2,
		THREE = TDS_3,
		FOUR = TDS_4,
		FIVE = TDS_5,
		SIX = TDS_6,
		SEVEN = TDS_7,
		EIGHT = TDS_8,
		NINE = TDS_9,
		MINUS = TDS_MINUS,
		PLUS = TDS_PLUS,

		SHIFT = TDS_SHIFT,
		CTRL = TDS_CONTROL,
		ALT = TDS_ALT,
		SPACE = TDS_SPACE,
		ESC = TDS_ESCAPE,
		CAPS = TDS_CAPSLOCK,
		TAB = TDS_TAB,
		ENTER = TDS_ENTER,
		BACKSPACE = TDS_BACKSPACE,
		TILDE = TDS_TILDE,
		COMMA = TDS_COMMA,
		PERIOD = TDS_PERIOD,
		SLASH = TDS_SLASH,
		SEMICOLON = TDS_SEMICOLON,

		M1 = TDS_MOUSE_LEFT,
		M2 = TDS_MOUSE_RIGHT,
		MIDDLE = TDS_MOUSE_MIDDLE,
		M4 = TDS_MOUSE_X1,
		M5 = TDS_MOUSE_X2,
		SCROLL = TDS_MOUSE_SCROLL,
	};*/

	public ref class Keycode
	{
	public:
		static uint32_t A = TDS_A;
		static uint32_t B = TDS_B;
		static uint32_t C = TDS_C;
		static uint32_t D = TDS_D;
		static uint32_t E = TDS_E;
		static uint32_t F = TDS_F;
		static uint32_t G = TDS_G;
		static uint32_t H = TDS_H;
		static uint32_t I = TDS_I;
		static uint32_t J = TDS_J;
		static uint32_t K = TDS_K;
		static uint32_t L = TDS_L;
		static uint32_t M = TDS_M;
		static uint32_t N = TDS_N;
		static uint32_t O = TDS_O;
		static uint32_t P = TDS_P;
		static uint32_t Q = TDS_Q;
		static uint32_t R = TDS_R;
		static uint32_t S = TDS_S;
		static uint32_t T = TDS_T;
		static uint32_t U = TDS_U;
		static uint32_t V = TDS_V;
		static uint32_t W = TDS_W;
		static uint32_t X = TDS_X;
		static uint32_t Y = TDS_Y;
		static uint32_t Z = TDS_Z;

		static uint32_t UP = TDS_UP;
		static uint32_t DOWN = TDS_DOWN;
		static uint32_t LEFT = TDS_LEFT;
		static uint32_t RIGHT = TDS_RIGHT;

		static uint32_t ZERO = TDS_0;
		static uint32_t ONE = TDS_1;
		static uint32_t TWO = TDS_2;
		static uint32_t THREE = TDS_3;
		static uint32_t FOUR = TDS_4;
		static uint32_t FIVE = TDS_5;
		static uint32_t SIX = TDS_6;
		static uint32_t SEVEN = TDS_7;
		static uint32_t EIGHT = TDS_8;
		static uint32_t NINE = TDS_9;
		static uint32_t MINUS = TDS_MINUS;
		static uint32_t PLUS = TDS_PLUS;

		static uint32_t SHIFT = TDS_SHIFT;
		static uint32_t CTRL = TDS_CONTROL;
		static uint32_t ALT = TDS_ALT;
		static uint32_t SPACE = TDS_SPACE;
		static uint32_t ESC = TDS_ESCAPE;
		static uint32_t CAPS = TDS_CAPSLOCK;
		static uint32_t TAB = TDS_TAB;
		static uint32_t ENTER = TDS_ENTER;
		static uint32_t BACKSPACE = TDS_BACKSPACE;
		static uint32_t TILDE = TDS_TILDE;
		static uint32_t COMMA = TDS_COMMA;
		static uint32_t PERIOD = TDS_PERIOD;
		static uint32_t SLASH = TDS_SLASH;
		static uint32_t SEMICOLON = TDS_SEMICOLON;

		static uint32_t MAX_KEYS = TDS_MAX_KEYS;

		static uint32_t M1 = TDS_MOUSE_LEFT + MAX_KEYS;
		static uint32_t M2 = TDS_MOUSE_RIGHT + MAX_KEYS;
		static uint32_t MIDDLE = TDS_MOUSE_MIDDLE + MAX_KEYS;
		static uint32_t M4 = TDS_MOUSE_X1 + MAX_KEYS;
		static uint32_t M5 = TDS_MOUSE_X2 + MAX_KEYS;
		static uint32_t SCROLL = TDS_MOUSE_SCROLL + MAX_KEYS;
	};

	public ref class Input
	{
	public:
		static void InputSetup();
		static float GetAxisRaw(System::String^ code);
		static float GetAxis(System::String^ code);
		static bool GetKey(uint32_t keycode);
		static bool GetKeyDown(uint32_t keycode);
		static bool GetKeyUp(uint32_t keycode);
		static bool GetMouseButton(uint32_t button);
		static bool GetMouseButtonUp(uint32_t button);
		static bool GetMouseButtonDown(uint32_t button);
		static float GetLocalMousePosX();
		static float GetLocalMousePosY();
		static float GetMousePositionDifferenceX();
		static float GetMousePositionDifferenceY();
		static void KeyRelease(uint32_t keycode);

		//new functions by amadeus for testing
		static void HideMouse();
		static void SetMousePosX(uint32_t Xpos);
		static void SetMousePosY(uint32_t Ypos);
		static void CenterMouseX();
		static void CenterMouseY();
		static void UpdateLastMousePos(int windowX, int windowY);
		static int GetScreenMouseX();
		static int GetScreenMouseY();
		static void CenterLastMousePos();
		static int ScreenMousePosdX();
		static int ScreenMousePosdY();


		//static float Get2DMousePosX();
		//static float Get2DMousePosY();
		static Vector3 GetMousePosition();
		
		

		static void InputUpdate();
		//static float mousePickedPositionX = TDS::Input::final_x_pos;
		//static float mousePickedPositionY = TDS::Input::final_y_pos;

	private:
		static System::Collections::Generic::Dictionary<System::String^, System::Tuple<uint32_t, uint32_t>^>^ AxisDictionary;
		static float mousePositionX;
		static float mousePositionY;
		static int WinPosX;
		static int WinPosY;

		static int LastMousePosX;
		static int LastMousePosY;
	};
}