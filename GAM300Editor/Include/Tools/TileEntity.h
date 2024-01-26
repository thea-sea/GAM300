#pragma once
#include "../Include/components/transform.h"
namespace TDS

{
	//typedef struct VkCommandBuffer_T* VkCommandBuffer;

	enum class PrimitiveShape : unsigned int
	{
		Empty,
		Triangle,
		Square
	};

	class GridProperties
	{
	public:
		GridProperties() :
			m_Position(Vec3(0.0f, 0.0f, 0.0f)),
			m_Rotation(Quat(1.0f, 0.0f, 0.0f, 0.0f)),
			m_Scale(Vec3(1.0f))
		{}

		void SetPosition(const Vec3& _position);
		void SetRotation(const Quat& _rotation);
		void SetScale(const Vec3& _scale);

		void Translate(const Vec3& _translation);
		void Rotate(const Quat& _rotation);
		void Scale(const Vec3& _scale);

		Mat4 GetModel() const;
		const Vec3& GetPosition() const { return m_Position; }
		const Vec3& GetScale() const { return m_Scale; }

	private:
		Vec3 m_Position;
		Quat m_Rotation;
		Vec3 m_Scale;
	};


	class TileEntity
	{
	public:
		TileEntity(const PrimitiveShape _primitiveShape = PrimitiveShape::Empty);
		~TileEntity();

		void SetSolidColor(const Vec3& _col) { m_Color = _col; }
		//void Draw(VkCommandBuffer _cmdBuffer, const uint32_t _numOfIndices) const;
		PrimitiveShape GetShape() const { return m_Shape; }
		const Vec3& GetColor() const { return m_Color; }

	public:
		Transform m_Transform;

	private:
		PrimitiveShape m_Shape;
		Vec3 m_Color;
	};


}

