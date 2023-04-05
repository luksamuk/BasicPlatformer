#pragma once
#include <OficinaFramework/ScreenSystem.hpp>
#include <OficinaFramework/AudioSystem.hpp>
#include <OficinaFramework/RenderingSystem.hpp>
#include "Parallax.hpp"

class TitleScreen : public OficinaFramework::ScreenSystem::Screen
{
private:

	int m_selection = 0,
		m_menuselection = 0,
		m_maxSelection = 4;
	float optionXPos;
	float menuSpeed = 30.0f;
	const std::string menuOptions[5] =
	{
		"    Game Start   >",
		"<  Level Select  >",
		"<     Extra      >",
		"<    Options     >",
		"<     Exit       "
	};

	OficinaFramework::AudioSystem::AudioSource* soundEmitter;
	OficinaFramework::AudioSystem::AudioSource* effectEmitter;
	OficinaFramework::AudioSystem::Audio*       bgmAudio;
	OficinaFramework::AudioSystem::Audio*       sfxNegate;

	OficinaFramework::RenderingSystem::Texture* titleLogo;
	OficinaFramework::RenderingSystem::Texture* titleLogo_black;
	OficinaFramework::RenderingSystem::Font*    menuFont;

	int m_fadetimer;
	float m_whitefade;
	float m_fade;
	Parallax parallax;
public:
	TitleScreen();
	// Inherited via Screen
	virtual void Initialize() override;
	virtual void LoadContent() override;
	virtual void UnloadContent() override;
	virtual void Update() override;
	virtual void Draw() override;
};
