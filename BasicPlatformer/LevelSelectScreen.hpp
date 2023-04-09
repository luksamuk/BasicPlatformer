#pragma once
#include <OficinaFramework/ScreenSystem.hpp>
#include <OficinaFramework/RenderingSystem.hpp>
#include <OficinaFramework/AudioSystem.hpp>
#include <string>

class LevelSelectScreen : public OficinaFramework::ScreenSystem::Screen
{
private:
	OficinaFramework::RenderingSystem::Font*    menuFont;
	OficinaFramework::RenderingSystem::Texture* bg_motif;
	OficinaFramework::RenderingSystem::Texture* bg_title;
	vec2dw                                      bg_motif_repeatcount;
	OficinaFramework::AudioSystem::Audio*       bgm;
	OficinaFramework::AudioSystem::AudioSource* soundEmitter;

	const std::string levelSelectOptions[25] =
	{
		"Isolated Island          1", // ID 00
		"                         2", // ID 01

		"Amazing Aquarium         1", // ID 02
		"                         2", // ID 03

		"ATTRACTION R2Z0          1", // ID 04
		"ATTRACTION R2Z1          2", // ID 05

		"ATTRACTION R3Z0          1", // ID 06
		"ATTRACTION R3Z1          2", // ID 07

		"ATTRACTION R4Z0          1", // ID 08
		"ATTRACTION R4Z1          2", // ID 09

		"Festival Freeway         1", // ID 10
		"                         2", // ID 11

		"Chaotic Corniche         1", // ID 12
		"                         2", // ID 13

		"ATTRACTION R7Z0          1", // ID 14
		"ATTRACTION R7Z1          2", // ID 15

		"ATTRACTION R8Z0          1", // ID 16
		"ATTRACTION R8Z1          2", // ID 17

		"ATTRACTION R9Z0           ", // ID 18

		"ATTRACTION R10Z0          ", // ID 19

		"Cosmic Colony            1", // ID 20
		"                         2", // ID 21

		"ATTRACTION R13Z0          ", // ID 22 -- playground is R12

		"Playground (R12Z0)        ",

		"Exit"
	};

	vec2 textPosition;
	int selection;
	int maxSelection = 25;
	int m_fadetype = 0;
	int m_inputdelay = 0;

	float m_fade = 1.0f;
public:
	LevelSelectScreen();
	// Inherited via Screen
	virtual void Initialize() override;
	virtual void LoadContent() override;
	virtual void UnloadContent() override;
	virtual void Update() override;
	virtual void Draw() override;
};
