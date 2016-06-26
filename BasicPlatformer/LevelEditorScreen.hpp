#pragma once
#include <OficinaFramework/ScreenSystem.hpp>
#include <OficinaFramework/RenderingSystem.hpp>
#include <OficinaFramework/AudioSystem.hpp>
#include "imgui_impl_sdl.hpp"

class LevelEditorScreen : public OficinaFramework::ScreenSystem::Screen
{
private:
    bool show_test_window = false;
    bool show_another_window = false;
    ImVec4 clear_color;
public:
	LevelEditorScreen();
	// Inherited via Screen
	virtual void Initialize() override;
	virtual void LoadContent() override;
	virtual void UnloadContent() override;
	virtual void Update() override;
	virtual void Draw() override;
};