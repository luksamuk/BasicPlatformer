#include "OptionsScreen.hpp"
#include "TitleScreen.hpp"
#include <OficinaFramework/InputSystem.hpp>
using namespace OficinaFramework;

OptionsScreen::OptionsScreen()
{
	SetVisible(true);
	SetActive(true);
}

void OptionsScreen::Initialize()
{
	// Deduce Window Size
	m_windowSizeOption = int((((float)ScreenSystem::GetWindowSize().x / (float)RenderingSystem::GetResolution().x) - 1.0f) / 0.5f);
	m_windowSizeOption = m_windowSizeOption < 0 ? 0 : m_windowSizeOption;
	m_fullscreenOption = ScreenSystem::IsFullScreen();
	m_filteringOption = RenderingSystem::GetLinearFilteringState();
	ScreenSystem::Screen::Initialize();
}

void OptionsScreen::LoadContent()
{
	menuFont = new RenderingSystem::Font(RenderingSystem::TexturePool::LoadTexture("fonts/levelselect"),
		vec2dw(8u), vec2b::One());
	bg_motif = RenderingSystem::TexturePool::LoadTexture("background/levelselect/levelselect");

	bg_motif_repeatcount = (RenderingSystem::GetResolution() / bg_motif->GetSize());
	bg_motif_repeatcount.x++; bg_motif_repeatcount.y++;

	soundEmitter = new AudioSystem::AudioSource;
	bgm = AudioSystem::AudioPool::LoadAudio("bgm/youre_my_hero", AudioSystem::OF_AUDIO_TYPE_OGG, true, 96.111f, 6.937f);
	ScreenSystem::Screen::LoadContent();
}

void OptionsScreen::UnloadContent()
{
	soundEmitter->Stop();
	RenderingSystem::TexturePool::DisposeTexture(bg_motif);
	AudioSystem::AudioPool::UnloadAudio(bgm);
	delete soundEmitter;
	delete menuFont;
	ScreenSystem::Screen::UnloadContent();
}

void OptionsScreen::Update()
{
	RenderingSystem::SetCameraPosition(RenderingSystem::GetResolution().toVec2() / 2.0f);

	// Debugger
	if (InputSystem::PressedKey(SDL_SCANCODE_F1)
		// Lstick + RB
		|| (InputSystem::PressingButton(InputSystem::GamePadButton::LSTICK)
			&& InputSystem::PressedButton(InputSystem::GamePadButton::RSHOULDER1)))
		ScreenSystem::SetDebug(!ScreenSystem::IsDebugActive());
	else if (InputSystem::PressedKey(SDL_SCANCODE_F2)
		|| InputSystem::PressedButton(InputSystem::GamePadButton::LSHOULDER1))
		ScreenSystem::Debug_ToggleMinimalist();
	
	// Change options
	if (m_inputdelay) m_inputdelay--;
	if (InputSystem::GetLeftStick().y > 0.0f && !m_inputdelay) {
		m_selection++;
		m_inputdelay = 15;
	}
	else if (InputSystem::GetLeftStick().y < 0.0f && !m_inputdelay) {
		m_selection--;
		m_inputdelay = 15;
	}
	// Wrap
	if (m_selection < 0) m_selection = m_maxSelection - 1;
	else if (m_selection >= m_maxSelection) m_selection = 0;
	// Select and change
	if (InputSystem::PressedButton(InputSystem::GamePadButton::A))
	{
		vec2 newWindowSize;
		switch (m_selection)
		{
		case 0: // Window Size
			m_windowSizeOption++;

			// Do not allow banned resolutions
			while ([&]() -> bool
			{
				for (int i = 0; i < m_bannedWindowSizesAmount; i++)
					if (m_windowSizeOption == bannedWindowSizeOptions[i])
						return true;
				return false;
			}()) m_windowSizeOption++;

			if (m_windowSizeOption >= m_windowSizeOptionsAmount)
				m_windowSizeOption = 0;

			break;
		case 1: // Full Screen
			m_fullscreenOption = !m_fullscreenOption;
			break;
		case 2: // Filtering
			m_filteringOption = !m_filteringOption;
			break;
		case 3: // Frame Rate
			break;
		case 4: // Apply

			// Apply Window Size
			newWindowSize = RenderingSystem::GetResolution().toVec2();
			newWindowSize *= 1.0f + (m_windowSizeOption * 0.5f);
			if (newWindowSize != ScreenSystem::GetWindowSize().toVec2())
			{
				ScreenSystem::SetWindowSize(vec2dw(static_cast<dword>(newWindowSize.x), static_cast<dword>(newWindowSize.y)));
				if (!ScreenSystem::IsFullScreen()) {
					RenderingSystem::DestroyDefaultBuffer();
					RenderingSystem::SetViewportSize(ScreenSystem::GetWindowSize());
					RenderingSystem::SetResolution(RenderingSystem::GetResolution());
					RenderingSystem::CreateDefaultBuffer();
				}
			}

			// Apply Fullscreen
			if (ScreenSystem::IsFullScreen() != m_fullscreenOption)
				ScreenSystem::SetFullScreen(m_fullscreenOption);

			// Apply Linear Filter
			if (RenderingSystem::GetLinearFilteringState() != m_filteringOption)
				RenderingSystem::SetLinearFiltering(m_filteringOption);

			break;
		case 5: // Back
			m_fadetype = 2;
			break;
		}
	}

	// Exit
	if (InputSystem::PressedButton(InputSystem::GamePadButton::B)) {
		m_selection = 5;
		m_fadetype = 2;
	}

	// Fade control
	switch (m_fadetype)
	{
	case 0: // Fade In
		m_fade -= 0.1f;
		clamp(m_fade, 0.0f, 1.0f);
		if (m_fade == 0.0f) {
			m_fadetype = 1;
			soundEmitter->Play(bgm);
		}
	case 1: // No Fade
		break;
	case 2: // Fade Out
		m_fade += 0.1f;
		clamp(m_fade, 0.0f, 1.0f);
		if (m_fade == 1.0f) {
			// Selection and etc
			ScreenSystem::AddScreen(new TitleScreen);
			RemoveMe();
		}
		break;
	}
}

void OptionsScreen::Draw()
{
	vec2 viewportPos = RenderingSystem::GetViewportPosition();
	vec2 viewportSize = RenderingSystem::GetResolution().toVec2();

	// Background
	for (dword i = 0u; i < bg_motif_repeatcount.x; i++)
		for (dword j = 0u; j < bg_motif_repeatcount.y; j++)
			bg_motif->Draw(vec2(float(bg_motif->GetSize().x * i), float(bg_motif->GetSize().y * j)), Color4::MaskToColor4(WHITE));

	// Options Title
	vec2 textSize = menuFont->MeasureString("* Options *", 1.5f);
	menuFont->DrawString(vec2((viewportSize.x / 2.0f) - (textSize.x / 2.0f), 20.0f), "* Options *", 1.5f);

	vec2 stringPosition(168.0f, 92.0f);

	// Frame
	glPushMatrix();
	glLineWidth(3.0f);
	glTranslatef(1.0f, 1.0f, 0.0f);
	RenderingSystem::glColorM(BLACK);
	glBegin(GL_LINE_LOOP);
	glVertex2f(stringPosition.x - 8.0f, stringPosition.y - 8.0f);
	glVertex2f(stringPosition.x + 304.0f, stringPosition.y - 8.0f);
	glVertex2f(stringPosition.x + 304.0f, stringPosition.y + 116.0f);
	glVertex2f(stringPosition.x - 8.0f, stringPosition.y + 116.0f);
	glEnd();
	glLineWidth(1.0f);
	glPopMatrix();

	glPushMatrix();
	glLineWidth(3.0f);
	//glTranslatef(2.0f, 2.0f, 0.0f);
	RenderingSystem::glColorM(WHITE);
	glBegin(GL_LINE_LOOP);
	glVertex2f(stringPosition.x - 8.0f, stringPosition.y - 8.0f);
	glVertex2f(stringPosition.x + 304.0f, stringPosition.y - 8.0f);
	glVertex2f(stringPosition.x + 304.0f, stringPosition.y + 116.0f);
	glVertex2f(stringPosition.x - 8.0f, stringPosition.y + 116.0f);
	glEnd();
	glLineWidth(1.0f);
	glPopMatrix();

	glPushMatrix();
	RenderingSystem::glTranslateToViewportPos();

	for (int i = 0; i < m_maxSelection; i++)
	{
		// Separator between Apply/Back and options
		if (i == m_maxSelection - 2) stringPosition.y += 24.0f;

		menuFont->DrawString(stringPosition, mainOptions[i], 1.0f, Color4::MaskToColor4(m_selection == i ? YELLOW : WHITE), 1.0f);

		switch (i)
		{
		case 0: // Window size
			menuFont->DrawString(vec2(stringPosition.x + 200.0f, stringPosition.y),
				windowSizeOptions[m_windowSizeOption],
				1.0f,
				Color4::MaskToColor4(m_fullscreenOption ? (m_selection == i ? GREY50 : GREY25) : (m_selection == i ? YELLOW : WHITE)),
				1.0f);
			break;
		case 1: // Full Screen
			menuFont->DrawString(vec2(stringPosition.x + 200.0f, stringPosition.y),
				triggerOptions[m_fullscreenOption],
				1.0f,
				Color4::MaskToColor4(m_selection == i ? YELLOW : WHITE),
				1.0f);
			break;
		case 2: // Filtering
			menuFont->DrawString(vec2(stringPosition.x + 200.0f, stringPosition.y),
				filteringOptions[m_filteringOption],
				1.0f,
				Color4::MaskToColor4(m_selection == i ? YELLOW : WHITE),
				1.0f);
			break;
		case 3: // Frame Rate
			menuFont->DrawString(vec2(stringPosition.x + 200.0f, stringPosition.y),
				framerateOptions[0],
				1.0f,
				Color4::MaskToColor4(m_selection == i ? GREY50 : GREY25),
				1.0f);
			break;
		default: break; // Apply / Back
		}

		stringPosition.y += 16.0f;
	}


	glPopMatrix();

	// Fade
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
