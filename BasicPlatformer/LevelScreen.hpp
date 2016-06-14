#pragma once
#include <OficinaFramework\ScreenSystem.hpp>
#include <OficinaFramework\EntitySystem.hpp>
#include <OficinaFramework\AudioSystem.hpp>

#include "Player.hpp"
#include "Solid.hpp"
#include "Parallax.hpp"

enum CurrentDayTime
{
	DAYTIME_MORNING,   // Mystic Cave 2P
	DAYTIME_AFTERNOON, // Walkin'
	DAYTIME_EVENING,   // Hyper-Hyper
	DAYTIME_NIGHT      // Evening Star
};

class LevelScreen :
	public OficinaFramework::ScreenSystem::Screen
{
private:
	dword LEVEL_ID;
	byte  LEVEL_ACT;
	std::string LEVEL_NAME;
	vec2 LEVEL_SIZE = vec2::One() * 1.0f;

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

	// Title card and HUD
	float m_fade;
	vec2  m_titlecard_pos;
	bool  m_timeractive = true;
	dword m_leveltimer  = 0u;
	std::string m_leveltimer_hud = " 0\'00\"00";
	OficinaFramework::RenderingSystem::SpriteSheet* hudSheet;
	OficinaFramework::RenderingSystem::Font*        hudFont;
	OficinaFramework::RenderingSystem::Font*        counterFont;
	OficinaFramework::RenderingSystem::Font*        titlecardFont;

	// Water
	bool  m_hasWater;
	float m_waterHeight;

	// Misc
	bool  m_clearcolorset;
	bool  m_paused = false;
	//Grid* m_grid;

	// Parallax
	Parallax parallax;
	Parallax fg_parallax;

	OficinaFramework::EntitySystem::DrawableEntityCollection m_drawables;

	Player* player;
public:
	LevelScreen(dword id);
	~LevelScreen();

	// Inherited via Screen
	virtual void Initialize() override;
	virtual void LoadContent() override;
	virtual void UnloadContent() override;
	virtual void Update() override;
	void updateCamera();
	virtual void Draw() override;
};

