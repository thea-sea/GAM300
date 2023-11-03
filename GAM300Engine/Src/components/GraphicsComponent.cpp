#include "components/graphicsComponent.h"

RTTR_REGISTRATION
{
	using namespace TDS;

	rttr::registration::class_<GraphicsComponent>("Graphics Component")
		.method("IsPointLight", &GraphicsComponent::IsPointLight)
		.method("TogglePointLight", &GraphicsComponent::TogglePointLight)
		.property("PointLight", &GraphicsComponent::m_Pointlight)
		.property("TextureName", &GraphicsComponent::m_TextureName)
		//.method("SetTextureName", &GraphicsComponent::SetTextureName)
		//.property("TextureName", &GraphicsComponent::m_TextureName, rttr::select_overload<void(std::string)>(&GraphicsComponent::SetTextureName))
		.property("ModelName", &GraphicsComponent::m_ModelName)
		.method("SetColor", rttr::select_overload<void(Vec4)>(&GraphicsComponent::SetColor))
		.method("SetColor", rttr::select_overload<void(float, float, float, float)>(&GraphicsComponent::SetColor))
		.method("GetColor", &GraphicsComponent::GetColor)
		.property("Color", &GraphicsComponent::m_Color);
		//.property("Color", &GraphicsComponent::GetColor, rttr::select_overload<void(Vec4)>(&GraphicsComponent::SetColor));
}

namespace TDS
{
	GraphicsComponent::GraphicsComponent(): m_AssetReference(),
											m_TextureReference(),
											m_ShowMesh(true),
											m_Color(1.f,1.f,1.f,1.f)
	{ }

	GraphicsComponent::GraphicsComponent(GraphicsComponent&& toMove) noexcept: m_AssetReference(toMove.m_AssetReference), 
																			   m_TextureReference(toMove.m_TextureReference), 
																			   m_ShowMesh(toMove.m_ShowMesh)
	{ }
}