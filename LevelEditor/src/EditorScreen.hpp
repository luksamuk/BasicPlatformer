#pragma once
#include <OficinaFramework/ScreenSystem.hpp>
#include <OficinaFramework/RenderingSystem.hpp>
#include <OficinaFramework/AudioSystem.hpp>
#include "imgui_impl_sdl.hpp"

#define sign(x) (x < 0.0f ? -1.0f : 1.0f)
#define SCROLLBAR_WIDTH 15.0f

#define CAMERA_MIN_X (RenderingSystem::GetResolution().x / 2.0f)
#define CAMERA_MIN_Y (RenderingSystem::GetResolution().y / 2.0f)
#define CAMERA_MAX_X (m_levelSize.x - CAMERA_MIN_X)
#define CAMERA_MAX_Y (m_levelSize.y - CAMERA_MIN_Y)
#define CAMERA_MIN_VEC (vec2(CAMERA_MIN_X,CAMERA_MIN_Y))
#define CAMERA_MAX_VEC (m_levelSize - CAMERA_MIN_VEC)

class EditorScreen : public OficinaFramework::ScreenSystem::Screen
{
private:
    bool show_test_window = false;
    bool show_another_window = false;
    vec2 m_cameraPosition;
	vec2 m_cameraSpeed;
	vec2 m_levelSize;
    ImVec4 clear_color;


	int theme = 0;
	int currentTheme = 0;

	void updateTheme();

	void drawGrid();
public:
	EditorScreen();
	// Inherited via Screen
	virtual void Initialize() override;
	virtual void LoadContent() override;
	virtual void UnloadContent() override;
	virtual void Update() override;
	virtual void Draw() override;
};