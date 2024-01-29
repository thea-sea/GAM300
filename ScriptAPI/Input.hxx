#pragma once
#include "IncludeFromEngine.hxx"
#include "Vector3.hxx"
#include "../GAM300Engine/Include/Input/Input.h"
//#include "../GAM300Engine/Include/Rendering/GraphicsManager.h"
#include "../GAM300Engine/Include/Input/InputSystem.h"
#include <algorithm>
#include "Vector2.hxx"

#define Mouse_X TDS::Input::getMousePosition()->x
#define Mouse_Y TDS::Input::getMousePosition()->y
#define TDSInput TDS::InputSystem::GetInstance();

namespace ScriptAPI
{
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

	public ref class VirtualKeyCodes
	{
	public:
		// Alphabets
		static uint32_t A = 0x41;
		static uint32_t B = 0x42;
		static uint32_t C = 0x43;
		static uint32_t D = 0x44;
		static uint32_t E = 0x45;
		static uint32_t F = 0x46;
		static uint32_t G = 0x47;
		static uint32_t H = 0x48;
		static uint32_t I = 0x49;
		static uint32_t J = 0x4A;
		static uint32_t K = 0x4B;
		static uint32_t L = 0x4C;
		static uint32_t M = 0x4D;
		static uint32_t N = 0x4E;
		static uint32_t O = 0x4F;
		static uint32_t P = 0x50;
		static uint32_t Q = 0x51;
		static uint32_t R = 0x52;
		static uint32_t S = 0x53;
		static uint32_t T = 0x54;
		static uint32_t U = 0x55;
		static uint32_t V = 0x56;
		static uint32_t W = 0x57;
		static uint32_t X = 0x58;
		static uint32_t Y = 0x59;
		static uint32_t Z = 0x5A;

		// Arrows
		static uint32_t UP = 0x26;
		static uint32_t DOWN = 0x28;
		static uint32_t LEFT = 0x25;
		static uint32_t RIGHT = 0x27;

		// Numbers
		static uint32_t NUM0 = 0x30;
		static uint32_t NUM1 = 0x31;
		static uint32_t NUM2 = 0x32;
		static uint32_t NUM3 = 0x33;
		static uint32_t NUM4 = 0x34;
		static uint32_t NUM5 = 0x35;
		static uint32_t NUM6 = 0x36;
		static uint32_t NUM7 = 0x37;
		static uint32_t NUM8 = 0x38;
		static uint32_t NUM9 = 0x39;

		// Special characters
		static uint32_t MINUS = 0xBD;    // -
		static uint32_t PLUS = 0xBB;     // +
		static uint32_t SHIFT = 0x10;
		static uint32_t LSHIFT = 0xA0;   // Left SHIFT
		static uint32_t RSHIFT = 0xA1;   // Right SHIFT
		static uint32_t CTRL = 0x11;
		static uint32_t LCTRL = 0xA2;   // Left CTRL
		static uint32_t RCTRL = 0xA3;   // Right CTRL
		static uint32_t ALT = 0x12;
		static uint32_t SPACE = 0x20;
		static uint32_t ESC = 0x1B;
		static uint32_t CAPS = 0x14;
		static uint32_t TAB = 0x09;
		static uint32_t ENTER = 0x0D;
		static uint32_t BACKSPACE = 0x08;
		static uint32_t TILDE = 0xC0;    // `
		static uint32_t COMMA = 0xBC;    // ,
		static uint32_t PERIOD = 0xBE;   // .
		static uint32_t SLASH = 0xBF;    // /
		static uint32_t SEMICOLON = 0xBA; // ;

		// Function keys (F1-F12)
		static uint32_t F1 = 0x70;
		static uint32_t F2 = 0x71;
		static uint32_t F3 = 0x72;
		static uint32_t F4 = 0x73;
		static uint32_t F5 = 0x74;
		static uint32_t F6 = 0x75;
		static uint32_t F7 = 0x76;
		static uint32_t F8 = 0x77;
		static uint32_t F9 = 0x78;
		static uint32_t F10 = 0x79;
		static uint32_t F11 = 0x7A;
		static uint32_t F12 = 0x7B;

		// Mouse buttons
		static uint32_t M1 = 0x01;
		static uint32_t M2 = 0x02;
		static uint32_t MIDDLE = 0x04;
		static uint32_t M4 = 0x05;
		static uint32_t M5 = 0x06;
		static uint32_t SCROLL = 0x07;

		// Maximum number of keys
		static uint32_t MAX_KEYS = 256;  // Total number of virtual key codes

		// Additional keys
		// Add more keys as needed
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


	// NEW INPUT CLASS STUFF
	public ref class InputSystem
	{
	public:
		//Get key is held
		static bool GetKey(uint32_t keycode);
		//Get key is pressed
		static bool GetKeyDown(uint32_t keycode);
		//Get key is released
		static bool GetKeyUp(uint32_t keycode);
		//Get mouse is held
		static bool GetMouseButton(uint32_t button);
		//Get mouse is pressed
		static bool GetMouseButtonDown(uint32_t button);
		//Get mouse is released
		static bool GetMouseButtonUp(uint32_t button);
		//Hide mouse
		static void HideMouse(bool value);
		//Lock the mouse in the center of the screen
		static void Lock(bool value);
		//Check mouse lock
		static bool isMouseLocked();

		static float GetLocalMousePosX();
		static float GetLocalMousePoxY();

		static float GetMouseDeltaX();
		static float GetMouseDeltaY();

		static float GetRawMouseAxisX();
		static float GetRawMouseAxisY();
	};
}

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
	};
*/
