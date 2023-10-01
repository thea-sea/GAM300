#include "camera/camera.h"
#include<iostream>
namespace TDS
{
	TDSCamera::TDSCamera(float Yaw, float pitch, Vec3 position, Vec3 up)
	{

		m_Position = position;
		m_WorldUp = up;
		m_Yaw = Yaw;
		m_Pitch = pitch;
		updateViewMatrix();
	}

	
	Mat4 TDSCamera::GetViewMatrix() const
	{
		return Mat4::LookAt(m_Position, m_Position + m_Front, m_Up);
	}


	void TDSCamera::updateViewMatrix()
	{
		// calculate the new Front vector
		Vec3 front;
		front.x = Mathf::Cos(m_Yaw * Mathf::Deg2Rad) * Mathf::Cos(m_Pitch * Mathf::Deg2Rad);
		front.y = Mathf::Sin(m_Pitch * Mathf::Deg2Rad);
		front.z = Mathf::Sin(m_Yaw * Mathf::Deg2Rad) * Mathf::Cos(m_Pitch * Mathf::Deg2Rad);
		m_Front = Vec3::Normalize(front);
		// also re-calculate the Right and Up vector
		m_Right = Vec3::Normalize(Vec3::Cross(m_Front, m_WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_Up = Vec3::Normalize(Vec3::Cross(m_Right, m_Front));

		m_Down = -m_Up;

		Input::wheelDelta = 0;
	}

	void TDSCamera::UpdateCamera(float deltaTime)
	{
	
		if (moving())
		{
			float CameraSpeed = m_Speed * deltaTime;
			if (keys.up)
			{
				m_Position += m_Front * CameraSpeed;
			}
			if(keys.down)
				m_Position -= m_Front * CameraSpeed;
			if(keys.left)
				m_Position -= m_Right * CameraSpeed;
			if(keys.right)
				m_Position += m_Right * CameraSpeed;
		}

		translate(static_cast<float>(Input::wheelDelta * 0.005f));

		static Input::mousePosition mouse = Input::mousePosition(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
		
		if (Input::isMouseButtonPressed(TDS_MOUSE_LEFT))
		{
			if (mouse.x == std::numeric_limits<int>::max() && mouse.y == std::numeric_limits<int>::max())
			{
				mouse = Input::getMousePosition();
			}

			if (!Input::isMouseButtonReleased(TDS_MOUSE_LEFT))
			{
				float GetMousex = mouse.x;
				float GetMousey = mouse.y;

				float getNewMousex = Input::getMousePosition().x;
				float getNewMousey = Input::getMousePosition().y;

				float offsetx = getNewMousex - GetMousex;
				float offsety = GetMousey - getNewMousey;

				ProcessMouseMovement(offsetx, offsety);

				mouse = Input::getMousePosition();
			}

		}
		else
		{
			mouse = Input::mousePosition(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
		}
		updateViewMatrix();
	}


	bool TDSCamera::moving()
	{
		if (Input::keystatus == Input::KeyStatus::PRESSED || Input::keystatus == Input::KeyStatus::REPEATED)
		{
			switch (Input::keyCode)
			{
				case TDS_W:
					return keys.up = true;
					break;

				case TDS_A:
					return keys.left = true;
					break;

				case TDS_S:
					return keys.down = true;
					break;

				case TDS_D:
					return keys.right = true;

				default:
					return false;
			}
		}
		else
		{
			keys.up		= false;		  
			keys.left	= false;				  
			keys.down	= false;			  
			keys.right	= false;
		}
		return false;
	}

	void TDSCamera::translate(const float& deltaWheel)
	{
		m_Fov -= deltaWheel;
		if (m_Fov < 1.f)
			m_Fov = 1.f;
		if (m_Fov > 45.f)
			m_Fov = 45.f;
	}

	void TDSCamera::ProcessMouseMovement(float  offsetX, float offsetY)
	{
		offsetX *= m_mouseSensitivity;
		offsetY *= m_mouseSensitivity;

		m_Yaw += offsetX;
		m_Pitch += offsetY;

		//prevent from out of bound
		if (m_Pitch > 89.f)
			m_Pitch = 89.f;
		if (m_Pitch < -89.f)
			m_Pitch = -89.f;

		updateViewMatrix();

	}
}
