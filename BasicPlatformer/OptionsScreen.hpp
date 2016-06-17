#pragma once
#include <OficinaFramework/ScreenSystem.hpp>
#include <OficinaFramework/RenderingSystem.hpp>
#include <OficinaFramework/AudioSystem.hpp>

class OptionsScreen : public OficinaFramework::ScreenSystem::Screen
{
private:
	OficinaFramework::RenderingSystem::Texture* bg_motif;
	vec2dw                                      bg_motif_repeatcount;
	OficinaFramework::AudioSystem::Audio*       bgm;
	OficinaFramework::AudioSystem::AudioSource* soundEmitter;
	OficinaFramework::RenderingSystem::Font*    menuFont;

	float m_fade = 1.0f;
	int   m_fadetype = 0;

	int m_selection = 0;
	const int m_maxSelection = 6;
	int m_inputdelay = 0;

	int m_windowSizeOption = 0;
	bool m_fullscreenOption;
	bool m_filteringOption;


	// Options menu
	const std::string mainOptions[6] =
	{
		"Resolution",
		"Full Screen",
		"Filter",
		"Frame Rate",

		"Apply Changes",
		"Back"
	};

	// Window size
	const std::string windowSizeOptions[11] =
	{
		"640x360",   // 1x
		"960x540",   // 1.5x - Looks bad. Don't use this
		"1280x720",  // 2x, 720p
		"1600x900",  // 2.5x
		"1920x1080", // 3x, 1080p
		"2240x1260", // 3.5x - Not an HD resolution. Don't use this
		"2560x1440", // 4x, 1440p
		"2880x1620", // 4.5x - Don't use this
		"3200x1800", // 5x   - Don't use this
		"3520x1980", // 5.5x - Not an HD resolution. Don't use this
		"3840x2160"  // 6x
	};
	const int m_windowSizeOptionsAmount = 11;
	const int bannedWindowSizeOptions[5] {
		1, 5, 7, 8, 9
	};
	const int m_bannedWindowSizesAmount = 5;

	// Full screen and trigger-type options
	const std::string triggerOptions[2] =
	{
		"Off",
		"On"
	};

	// Linear filter
	const std::string filteringOptions[2] =
	{
		"Normal",
		"Linear"
	};

	// FPS
	const std::string framerateOptions[2] =
	{
		"Capped 60FPS",
		"Vertical Synchro"
	};

public:
	OptionsScreen();

	// Inherited via Screen
	virtual void Initialize() override;
	virtual void LoadContent() override;
	virtual void UnloadContent() override;
	virtual void Update() override;
	virtual void Draw() override;

};