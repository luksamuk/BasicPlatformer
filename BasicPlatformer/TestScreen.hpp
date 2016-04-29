#pragma once
#include <OficinaFramework\ScreenSystem.hpp>
#include <OficinaFramework\EntitySystem.hpp>
#include <OficinaFramework\AudioSystem.hpp>

#include "Player.hpp"
#include "Solid.hpp"

enum CurrentDayTime
{
	DAYTIME_MORNING,   // Mystic Cave 2P
	DAYTIME_AFTERNOON, // Walkin'
	DAYTIME_EVENING,   // Hyper-Hyper
	DAYTIME_NIGHT      // Evening Star
};

class TestScreen :
	public OficinaFramework::ScreenSystem::Screen
{
private:
	OficinaFramework::AudioSystem::Audio*       bgm;
	OficinaFramework::AudioSystem::AudioSource* soundemitter;
	CurrentDayTime                              dayTime;

	// Camera stuff
	const float m_camerabound_x_min_factor     = -0.45f;
	const float m_camerabound_x_max_factor     =  0.5f;
	const float m_camerabound_y_min_factor_air = -0.26f;
	const float m_camerabound_y_max_factor_air =  0.53f;
	const float m_camerabound_y_factor         =  0.4f;
	const float m_camera_move_speed            = 16.0f;
	const float m_camera_move_speed_Yground    =  6.0f;
	int   m_cameralag;
	bool  m_playerSpindashed;

	// Title card
	float m_fade;

	// Water
	float m_waterHeight;

	// Misc
	bool  m_clearcolorset;
	//Grid* m_grid;

	OficinaFramework::EntitySystem::DrawableEntityCollection m_drawables;

	Player* player;
public:
	TestScreen();
	~TestScreen();

	// Inherited via Screen
	virtual void Initialize() override;
	virtual void LoadContent() override;
	virtual void UnloadContent() override;
	virtual void Update() override;
	void updateCamera();
	virtual void Draw() override;
};

