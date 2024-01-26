/*!*************************************************************************
****
\file Input.h
\author Cheung Jun Yin Matthew
\par DP email: j.cheung@digipen.edu
\par Course: CSD3400
\par Section: a
\par assignment: M1
\date 01-10-2023
\brief  This file implements the input handling system for the TDS engine.
		Input mapping is implemented via win32 API.
****************************************************************************
***/

#ifndef TDS_INPUTS
#define TDS_INPUTS
#include "windowswindow.h"
#include <windowsx.h>
#include <stdint.h>

#include "dotnet/ImportExport.h"
#define TDS_MAX_KEYS 56

#define TDS_A			0
#define TDS_B			1
#define TDS_C			2
#define TDS_D			3
#define TDS_E			4
#define TDS_F			5
#define TDS_G			6
#define TDS_H			7
#define TDS_I			8
#define TDS_J			9
#define TDS_K			10
#define TDS_L			11
#define TDS_M			12
#define TDS_N			13
#define TDS_O			14
#define TDS_P			15
#define TDS_Q			16
#define TDS_R			17
#define TDS_S			18
#define TDS_T			19
#define TDS_U			20
#define TDS_V			21
#define TDS_W			22
#define TDS_X			23
#define TDS_Y			24
#define TDS_Z			25

#define TDS_UP			26
#define TDS_DOWN		27
#define TDS_LEFT		28
#define TDS_RIGHT		29

#define TDS_0			30
#define TDS_1			31
#define TDS_2			32
#define TDS_3			33
#define TDS_4			34
#define TDS_5			35
#define TDS_6			36
#define TDS_7			37
#define TDS_8			38
#define TDS_9			39
#define TDS_MINUS		40
#define TDS_PLUS		41

#define TDS_SHIFT		42
#define TDS_CONTROL		43
#define TDS_ALT			44
#define TDS_SPACE		45
#define TDS_ESCAPE		46
#define TDS_CAPSLOCK	47
#define TDS_TAB			48
#define TDS_ENTER		49
#define TDS_BACKSPACE	50
#define TDS_TILDE		51
#define TDS_COMMA		52
#define TDS_PERIOD		53
#define TDS_SLASH		54
#define TDS_SEMICOLON	55

#define TDS_MAX_MOUSE_BUTTONS 7

#define TDS_MOUSE_LEFT		 0
#define TDS_MOUSE_RIGHT		 1
#define TDS_MOUSE_MIDDLE	 2
#define TDS_MOUSE_X1		 3
#define TDS_MOUSE_X2		 4
#define TDS_MOUSE_SCROLL	 5
#define TDS_MOUSE_HOVER		 6


namespace TDS
{
	class DLL_API Input
	{

	public:
		struct keyState
		{
			bool wasDown, isDown;
		};
		enum class KeyStatus
		{
			IDLE,
			PRESSED,
			RELEASED,
			REPEATED
		};
		struct keyboardInputMap
		{
			keyState keys[TDS_MAX_KEYS];
		};

		struct buttonState
		{
			bool wasDown, isDown, isScrollUp, isScrollDown;
		};

		struct mousePosition
		{
			int x, y;
		};

		struct mouseInputMap
		{
			buttonState buttons[TDS_MAX_MOUSE_BUTTONS];
			mousePosition position;
		};

		static KeyStatus keystatus;
		static uint32_t keyCode;
		static short wheelDelta;
	private:
		static keyboardInputMap keyboard;
		static mouseInputMap mouse;
	public:

		/*!*************************************************************************
		Return the state of the key, wasDown, IsDown.
		****************************************************************************/
		static keyState GetKeyState(uint32_t keycode);

		/*!*************************************************************************
		Returns true if key is being pressed.
		****************************************************************************/
		static bool isKeyPressed(uint32_t keycode);

		/*!*************************************************************************
		Returns true if key is released.
		****************************************************************************/
		static bool isKeyReleased(uint32_t keycode);

		/*!*************************************************************************
		Returns true if key key has just been pressed.
		****************************************************************************/
		static bool wasKeyHit(uint32_t keycode);

		/*!*************************************************************************
		Manaually Reset KeyPress
		****************************************************************************/
		static void KeyRelease(uint32_t keycode);

		/*!*************************************************************************
		Returns true if mouse wheel is being scrolled up.
		****************************************************************************/
		static bool isMouseScrollUp();

		/*!*************************************************************************
		Returns true if mouse wheel is being scrolled down.
		****************************************************************************/
		static bool isMouseScrollDown();

		/*!*************************************************************************
		Returns mouse position
		****************************************************************************/
		static mousePosition getMousePosition();

		/*!*************************************************************************
		Returns true if mouse button is being pressed.
		****************************************************************************/
		static bool isMouseButtonPressed(unsigned int buttonCode);

		/*!*************************************************************************
		Returns true if mouse button is being released.
		****************************************************************************/
		static bool isMouseButtonReleased(unsigned int buttonCode);

		static void releaseTheMouse(unsigned int buttonCode);

		/*!*************************************************************************
		Returns true if button has just been pressed.
		****************************************************************************/
		static bool wasMouseButtonHit(unsigned int buttonCode);

		/*!*************************************************************************
		Processes mouse scroll bools through GET_WHEEL_DELTA_WPARAM
		****************************************************************************/
		static void processMouseScroll(WPARAM _wParam);

		/*!*************************************************************************
		Stops scroll wheel from continuously outputing scroll up or down data.
		****************************************************************************/
		static void scrollStop();

		/*!*************************************************************************
		Maps the keycodes to the macros defined by offseting them from the win32 API
		keycode numbers.
		****************************************************************************/
		static void processKeyboardInput(uint32_t VKcode, bool wasDown, bool isDown);

		/*!*************************************************************************
		Updates the mouse button bools
		****************************************************************************/
		static void processMouseInput(WPARAM wParam, LPARAM lParam);

		/*!*************************************************************************
		Updates the mouse position through GET_X_LPARAM(lParam) & GET_Y_LPARAM(lParam)
		to track global position of the mouse.
		****************************************************************************/
		static void updateMousePosition(LPARAM lParam);

		/*!*************************************************************************
		Releases the key after it is pressed.
		****************************************************************************/
		static void releaseTheKey(uint32_t key);

		/*!*************************************************************************
		Set the center position for the mouse.
		****************************************************************************/
		static void setCenteredMouse(float x, float y);

		static void setExitCursor(bool input);
		static bool getExitCursor();

		static void setLocalMousePos(Vec2 mousePos);
		static Vec2 getLocalMousePos();

		static float getLocalMousePosX();
		static float getLocalMousePosY();

		static void centerandhidemouse(HWND hwnd);
		//static inline float final_x_pos;
		//static inline float final_y_pos;
		//static float GetObjectPickPosX();
		//static float GetObjectPickPosY();

		static inline float mX;
		static inline float mY;
		static inline float mWidth;
		static inline float mHeight;

	private:
		static bool exit_cursor;
		static Vec2 local_MousePos;

	}; //end of Input class

} //end of namespace

#endif