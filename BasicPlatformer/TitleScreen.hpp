#pragma once
#include <OficinaFramework\ScreenSystem.hpp>
#include <OficinaFramework\AudioSystem.hpp>
#include <OficinaFramework\RenderingSystem.hpp>

class TitleScreen : public OficinaFramework::ScreenSystem::Screen
{
private:

	int m_selection = 0,
		m_menuselection = 0,
		m_maxSelection = 3;
	float optionXPos;
	float menuSpeed = 30.0f;
	const std::string menuOptions[4] =
	{
		"  Level Select >",
		"< Level Editor >",
		"<    Options   >",
		"<     Quit      "
	};

	OficinaFramework::AudioSystem::AudioSource* soundEmitter;
	OficinaFramework::AudioSystem::Audio*       bgmAudio;

	OficinaFramework::RenderingSystem::Texture* titleLogo;
	OficinaFramework::RenderingSystem::Texture* titleLogo_black;
	OficinaFramework::RenderingSystem::Font*    menuFont;

	int m_fadetimer;
	float m_whitefade;
	float m_fade;
public:
	TitleScreen();
	// Inherited via Screen
	virtual void Initialize() override;
	virtual void LoadContent() override;
	virtual void UnloadContent() override;
	virtual void Update() override;
	virtual void Draw() override;
};