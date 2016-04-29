#pragma once
#include <OficinaFramework\ScreenSystem.hpp>
#include <OficinaFramework\RenderingSystem.hpp>
#include <OficinaFramework\AudioSystem.hpp>
#include <string>

class TitleScreen : public OficinaFramework::ScreenSystem::Screen
{
private:
	OficinaFramework::RenderingSystem::Font*    menuFont;
	OficinaFramework::RenderingSystem::Texture* bg_motif;
	vec2dw                                      bg_motif_repeatcount;
	OficinaFramework::AudioSystem::Audio*       bgm;
	OficinaFramework::AudioSystem::AudioSource* soundEmitter;

	const std::string menuOptions[4] =
	{
		"Level Select",
		"Level Editor",
		"Options",
		"Quit"
	};

	const std::string levelSelectOptions[15] =
	{
		"Engine Epitomy           1",
		"                         2",

		"Aquatic Attraction       1",
		"                         2",

		"Freezing Factory         1",
		"                         2",

		"White Wonder             1",
		"                         2",

		"Dusty Desert             1",
		"                         2",

		"Jester Juggle            1",
		"                         2",

		"Cyber City               1",
		"                         2",

		"Exit"
	};

	vec2 textPosition;
	int selection;
	int maxSelection = 15;
	int m_fadetype = 0;

	float m_fade = 1.0f;
public:
	TitleScreen();
	// Inherited via Screen
	virtual void Initialize() override;
	virtual void LoadContent() override;
	virtual void UnloadContent() override;
	virtual void Update() override;
	virtual void Draw() override;
};