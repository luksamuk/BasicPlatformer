#pragma once
#include <OficinaFramework/ScreenSystem.hpp>
#include <OficinaFramework/RenderingSystem.hpp>
using namespace OficinaFramework;

class PresentationScreen : public ScreenSystem::Screen {
private:
	RenderingSystem::Texture*     m_poweredby_texture;
	RenderingSystem::SpriteSheet* m_poweredby_sheet;
	RenderingSystem::Animation*   m_poweredby;
	float m_timer;
public:
	void Initialize() override;
	void LoadContent() override;
	void UnloadContent() override;
	void Update() override;
	void Draw() override;
};