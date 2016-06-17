#pragma once
#include <OficinaFramework/ScreenSystem.hpp>
#include <OficinaFramework/RenderingSystem.hpp>
using namespace OficinaFramework;


class LoadingScreen : public ScreenSystem::Screen
{
private:
	float angle = 0.0f;
	RenderingSystem::Font* font;
public:
	void Initialize() override;
	void LoadContent() override;
	void UnloadContent() override;
	void Update() override;
	void Draw() override;
};