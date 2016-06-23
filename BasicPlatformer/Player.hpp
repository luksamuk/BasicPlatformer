#pragma once
#include <OficinaFramework/EntitySystem.hpp>
#include <OficinaFramework/InputSystem.hpp>
#include <OficinaFramework/ScreenSystem.hpp>
#include <OficinaFramework/AudioSystem.hpp>
#include "Grid.hpp"
#include "EffectSpawner.hpp"

struct PlayerValues
{
	float m_gravity;
	float m_accel;
	float m_airaccel;
	float m_friction;
	float m_decel;
	float m_maxXSpeed;
	float m_topXSpeed;
	float m_jump_strength;
	float m_min_jump_str;
	float m_airdragfactor;
	float m_airdrag_minxv;
	float m_airdrag_minyv;
	float m_slopefactor;

	float m_rollingfriction;
	float m_rollingdecel;
	float m_rollingTopXSpeed;
	float m_rollingMinXSpeed;
	float m_unrollMinXSpeed;
};

struct PlayerSFX
{
	OficinaFramework::AudioSystem::Audio
		* s00_skidding,
		* s01_rolling,
		* s02_jump,
		* s03_spindash,
		* s04_release,
		* s05_water,
		* s06_normalshield,
		* s07_bubbleshield,
		* s08_watercount;
};

enum PlayerValueState
{
	PLAYER_DEFAULT,
	PLAYER_SPEEDSHOES,
	PLAYER_UNDERWATER,
	PLAYER_SUPER,
	PLAYER_SUPER_UNDERWATER
};

enum PlayerActionState
{
	PLAYER_NOACTION,
	PLAYER_JUMPING,
	PLAYER_SKIDDING,
	PLAYER_ROLLING,
	PLAYER_CROUCHED,
	PLAYER_LOOKUP,
	PLAYER_SPINDASH
};

enum ShieldType
{
	SHIELD_NONE,
	SHIELD_NORMAL,
	SHIELD_BUBBLE
};

class Player :
	public OficinaFramework::EntitySystem::DrawableEntity
{
private:

	// Default values for Player
	const PlayerValues default_values = {
		0.21875f,   // Gravity
		0.046875f,  // Acceleration
		0.09375f,   // Air Acceleration
		0.046875f,  // Friction
		0.5f,       // Deceleration
		12.0f,      // Max X Speed
		6.0f,       // Top X Speed
		-6.5f,      // Jump Strength
		-4.0f,      // Minimum Jump Strength
		0.96875f,   // Air Drag Factor
		0.0125f,    // Air Drag Minimum X value
		-4.0f,      // Air Drag Minimum Y value
		0.125f,     // Slope Factor

		0.0234375f, // Rolling Friction
		0.125f,     // Rolling Deceleration
		16.0f,      // Rolling Top X Speed
		1.03125f,   // Rolling Minimum X Speed
		0.5f        // Unroll Minimum X Speed
	};

	// Speed Shoes
	const PlayerValues speedshoes_values = {
		0.21875f,
		0.09375f,    // Acceleration
		0.1875f,     // Air Acceleration
		0.09375f,    // Friction
		0.5f,
		12.0f,
		12.0f,
		-6.5f,
		-4.0f,
		0.96875f,
		0.0125f,
		-4.0f,
		0.125f,

		0.046875f,  // Rolling Friction
		0.125f,
		16.0f,
		1.03125f,
		0.5f
	};

	const PlayerValues underwater_values = {
		0.0625f,     // Gravity
		0.0234375f,  // Acceleration
		0.046875f,   // Air Acceleration
		0.0234375f,  // Friction
		0.25f,       // Deceleration
		12.0f,	     
		3.0f,        // Top X Speed
		-3.5f,
		-2.0f,
		0.96875f,
		0.0125f,
		-4.0f,
		0.125f,

		0.01171875f, // Rolling Friction
		0.125f,
		16.0f,
		1.03125f,
		0.5f
	};

	const PlayerValues super_values = {
		0.21875f,
		0.1875f,   // Acceleration
		0.375f,    // Air Acceleration
		0.046875f,
		1.0f,      // Deceleration
		12.0f,
		10.0f,     // Top X Speed
		-8.0f,     // Jump Strength
		-4.0f,
		0.96875f,
		0.0125f,
		-4.0f,
		0.125f,

		0.0234375f,
		0.125f,
		16.0f,
		1.03125f,
		0.5f
	};

	const PlayerValues super_underwater_values = {
		0.0625f,   // Gravity
		0.09375f,  // Acceleration
		0.1875f,   // Air Acceleration
		0.046875f, // Friction
		0.5f,      // Deceleration
		12.0f,
		5.0f,      // Top X Speed
		-3.5f,
		-2.0f,
		0.96875f,
		0.0125f,
		-4.0f,
		0.125f,

		0.0234375f,
		0.125f,
		16.0f,
		1.03125f,
		0.5f
	};

	// State related
	const float m_hitboxRadius  = 20.0f;
	float       m_hitboxHeight  = m_hitboxRadius;
	PlayerValueState              m_currentState;
	const PlayerValues* values;

	// Collision related
	const float m_vlinecast                = m_hitboxRadius + 16.0f;
	float m_vlinecast_top                  = m_hitboxHeight + 16.0f;
	const float m_hlinecast                = 10.0f;
	const float m_sidesensors_ydist        = 4.0f;
	const float m_vertsensor_dist          = 9.0f;
	Grid*       m_grid;
	
	// Movement related
	vec2        m_velocity;
	vec2        m_groundvelocity;
	float       m_direction;
	bool        ground;
	bool        ceiling;
	bool        leftwall;
	bool        rightwall;
	bool        pressingJump;
	float       jumpAngle;

	// Sensor positions
	vec2 m_leftSensor,
		m_rightSensor,
		m_bottomSensorL,
		m_bottomSensorR,
		m_topSensorL,
		m_topSensorR;

	// Sound
	OficinaFramework::AudioSystem::AudioSource* soundEmitter;
	PlayerSFX                                   sfx = { 0 };
	OficinaFramework::AudioSystem::Audio*       sfx_00_skidding;
	OficinaFramework::AudioSystem::Audio*       sfx_01_rolling;
	OficinaFramework::AudioSystem::Audio*       sfx_02_jump;
	OficinaFramework::AudioSystem::Audio*       sfx_03_spindash;
	OficinaFramework::AudioSystem::Audio*       sfx_04_release;
	OficinaFramework::AudioSystem::Audio*       sfx_05_water;

	// Action triggers
	bool                          m_viewsensors;
	bool                          m_super;
	bool                          m_underwater;
	bool                          m_speedshoes;

	// Animation and Action
	OficinaFramework::RenderingSystem::Texture*     t_sonic;
	OficinaFramework::RenderingSystem::Texture*     t_supersonic;
	OficinaFramework::RenderingSystem::Animation*   m_animator;
	OficinaFramework::RenderingSystem::Animation*   sonicAnimator;
	OficinaFramework::RenderingSystem::Animation*   superAnimator;
	OficinaFramework::RenderingSystem::SpriteSheet* spriteSheet;
	OficinaFramework::RenderingSystem::SpriteSheet* superSheet;
	int                                             m_animationtimer;
	PlayerActionState                               m_currentAction;
	float                                           m_spindashRevolutions;
	float                                           m_xSpeedBeforeJump;

	// Water related
	bool                                            m_haswater;
	float                                           m_waterHeight;

	// Effect related
	EffectSpawner* 									m_spawner;
	byte                                            m_superspark_span = 0u;
	byte                                            m_minibubble_span = 120u;
	word                                            m_drown_span      = 1800u;
	ShieldType                                      m_currentshield;
	OficinaFramework::EntitySystem::DrawableEntity* m_shieldhandle = nullptr;

public:
	Player();
	~Player();

	bool OnGround();
	vec2 Speed();
	vec2 GroundSpeed();

	// Inherited via DrawableEntity
	virtual void Initialize() override;
	virtual void Update() override;
	void updateAnimation(float leftStickX);
	void reset();
	void setAnimation(std::string);
	vec2 getGSpeed();
	void setGSpeed(vec2);
	PlayerActionState getAction();
	PlayerValueState  getState();
	void setAction(PlayerActionState);
	float getSpindashRev();
	bool* getViewSensorsPtr();

	void setWaterHeight(float f);
	void disableWater();
	void setGrid(Grid*);
	void disableGrid();
	void setSpawner(EffectSpawner*);

	virtual void LoadContent() override;
	virtual void UnloadContent() override;
	virtual void Draw() override;
};

