#include "TitleScreen.hpp"
#include "LevelSelectScreen.hpp"
#include <OficinaFramework\InputSystem.hpp>
using namespace OficinaFramework;

TitleScreen::TitleScreen()
{
	SetActive(true);
	SetVisible(true);
}

void TitleScreen::Initialize()
{
	m_fadetimer = 0;
	m_whitefade = 1.0f;
	m_fade = 0.0f;
	ScreenSystem::Screen::Initialize();
}

void TitleScreen::LoadContent()
{
	soundEmitter = new AudioSystem::AudioSource;
	bgmAudio = AudioSystem::AudioPool::LoadAudio("bgm/titlescreen", AudioSystem::OF_AUDIO_TYPE_OGG);
	titleLogo = RenderingSystem::TexturePool::LoadTexture("background/titlescreen/title");
	titleLogo_black = RenderingSystem::TexturePool::LoadTexture("background/titlescreen/title_black");
	menuFont = new RenderingSystem::Font(RenderingSystem::TexturePool::LoadTexture("fonts/levelselect"),
		vec2dw(8u), vec2b::One());

	optionXPos = (RenderingSystem::GetResolution().toVec2().x / 2.0f) - (menuFont->MeasureString(menuOptions[0], 1.5f).x / 2.0f);

	ScreenSystem::Screen::LoadContent();
}

void TitleScreen::UnloadContent()
{
	delete menuFont;
	RenderingSystem::TexturePool::DisposeTexture(titleLogo);
	soundEmitter->Stop();
	AudioSystem::AudioPool::UnloadAudio(bgmAudio);
	ScreenSystem::Screen::UnloadContent();
}

void TitleScreen::Update()
{
	RenderingSystem::SetCameraPosition(RenderingSystem::GetResolution().toVec2() / 2.0f);

	// Debugger
	if (InputSystem::PressedKey(SDL_SCANCODE_F1))
		ScreenSystem::SetDebug(!ScreenSystem::IsDebugActive());
	else if (InputSystem::PressedKey(SDL_SCANCODE_F2))
		ScreenSystem::Debug_ToggleMinimalist();
	// Fullscreen toggle
	else if (InputSystem::PressedKey(SDL_SCANCODE_F11))
		ScreenSystem::SetFullScreen(!ScreenSystem::IsFullScreen());
	// Linear filter toggle
	else if (InputSystem::PressedKey(SDL_SCANCODE_F5))
		RenderingSystem::SetLinearFiltering(!RenderingSystem::GetLinearFilteringState());

	// Fade out + transition
	if (m_fade > 0.0f && m_fade < 1.0f)
		m_fade += 0.1f;
	else if (m_fade > 1.0f)
	{
		m_fade = 1.0f;
		switch (m_selection)
		{
		case 0: // Level Select
			ScreenSystem::AddScreen(new LevelSelectScreen);
			RemoveMe();
			break;
		case 3: // Quit
			InputSystem::CallExitCommand();
			break;
		default: break;
		}
	}

	// Option select logic
	if (m_menuselection != m_selection)
	{
		// Option logic
		if (m_menuselection < m_selection)
		{
			optionXPos -= menuSpeed;
			if (optionXPos < -menuFont->MeasureString(menuOptions[m_menuselection], 1.5f).x)
			{
				m_menuselection = m_selection;
				optionXPos = RenderingSystem::GetResolution().toVec2().x;
			}
		}
		else if (m_menuselection > m_selection)
		{
			optionXPos += menuSpeed;
			if (optionXPos > float(RenderingSystem::GetResolution().toVec2().x))
			{
				m_menuselection = m_selection;
				optionXPos = -menuFont->MeasureString(menuOptions[m_menuselection], 1.5f).x;
			}
		}
	}
	else
	{
		// Change option
		float optionSize = menuFont->MeasureString(menuOptions[m_menuselection], 1.5f).x;
		float optionFinalPos = (RenderingSystem::GetResolution().toVec2().x / 2.0f) - (optionSize / 2.0f);
		
		if (abs(optionXPos - optionFinalPos) < menuSpeed)
			optionXPos = optionFinalPos;
		if (optionXPos < optionFinalPos)
		{
			optionXPos += menuSpeed;
		}
		else if (optionXPos > optionFinalPos)
		{
			optionXPos -= menuSpeed;
		}
		else
		{
			if (InputSystem::GetLeftStick().x > 0.0f)
				m_selection++;
			else if (InputSystem::GetLeftStick().x < 0.0f)
				m_selection--;
		}
		if (m_selection < 0) m_selection = 0;
		if (m_selection > m_maxSelection) m_selection = m_maxSelection;
	}

	// White intro
	if(!m_fadetimer) soundEmitter->Play(bgmAudio);
	if (m_fadetimer < 42)
	{
		m_fadetimer++;
		if (InputSystem::PressedButton(InputSystem::GamePadButton::A))
			m_fadetimer = 42;
	}
	else if(m_fadetimer >= 42)
	{
		if (m_whitefade > 0.0f) m_whitefade -= 0.1f;

		if (InputSystem::PressedButton(InputSystem::GamePadButton::A)
			&& m_fade == 0.0f
			&& (m_selection == m_menuselection))
		{
			// For now, only Level Select and Quit are available
			if(m_selection == 0 || m_selection == m_maxSelection)
				m_fade = 0.1f;
		}
	}
}

void TitleScreen::Draw()
{
	vec2 viewportPos = RenderingSystem::GetViewportPosition();
	vec2 viewportSize = RenderingSystem::GetResolution().toVec2();

	RenderingSystem::glClearColorM(m_fadetimer < 42 ? WHITE : BLACK);

	// Logo management
	RenderingSystem::Texture* currentLogo = m_fadetimer < 42 ? titleLogo_black : titleLogo;
	float scale = m_fadetimer < 42 ? float(m_fadetimer) / 42.0f : 1.0f;
	
	// Draw white background
	if (m_fadetimer >= 42 && m_whitefade > 0.0f)
	{
		glPushMatrix();
		glTranslatef(viewportPos.x, viewportPos.y, 0.0f);
		OficinaFramework::RenderingSystem::glColorM(WHITE, m_whitefade);
		glBegin(GL_QUADS);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(viewportSize.x, 0.0f);
		glVertex2f(viewportSize.x, viewportSize.y);
		glVertex2f(0.0f, viewportSize.y);
		glEnd();
		glPopMatrix();
	}

	// Draw logo
	currentLogo->Draw(vec2(RenderingSystem::GetResolution().toVec2() / 2.0f) - vec2(currentLogo->GetSize().toVec2() * (scale / 2.0f)),
		Color4::MaskToColor4(WHITE), scale);

	// Draw text
	if (m_fadetimer >= 42) {
		// Option Select
		vec2 optionPos = vec2(RenderingSystem::GetResolution().toVec2() / 2.0f);
		optionPos.x = optionXPos;
		//optionPos.x -= menuFont->MeasureString(menuOptions[m_menuselection], 1.5f).x / 2.0f;
		optionPos.y += optionPos.y - 40.0f;
		menuFont->DrawString(optionPos, menuOptions[m_menuselection], 1.5f);

		// Disclaimer
		menuFont->DrawString(viewportSize - menuFont->MeasureString("           2016 luksamuk\nNot Affiliated with SEGA", 1.0f),
			"           2016 luksamuk\nNot Affiliated with SEGA");
	}

	// Fade out
	if (m_fade > 0.0f)
	{
		glPushMatrix();
		glTranslatef(viewportPos.x, viewportPos.y, 0.0f);
		OficinaFramework::RenderingSystem::glColorM(BLACK, m_fade > 1.0f ? 1.0f : m_fade);
		glBegin(GL_QUADS);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(viewportSize.x, 0.0f);
		glVertex2f(viewportSize.x, viewportSize.y);
		glVertex2f(0.0f, viewportSize.y);
		glEnd();
		glPopMatrix();
	}
}
