#include "LevelSelectScreen.hpp"
#include "TitleScreen.hpp"
#include "LevelScreen.hpp"
#include <OficinaFramework\InputSystem.hpp>
using namespace OficinaFramework;

LevelSelectScreen::LevelSelectScreen()
{
	SetVisible(true);
	SetActive(true);
}

void LevelSelectScreen::Initialize()
{
	textPosition = vec2((RenderingSystem::GetResolution().x / 4.0f) - 104.0f, 60.0f);
	selection = 0;
	ScreenSystem::Screen::Initialize();
}

void LevelSelectScreen::LoadContent()
{
	menuFont = new RenderingSystem::Font(RenderingSystem::TexturePool::LoadTexture("fonts/levelselect"),
		vec2dw(8u), vec2b::One());
	bg_motif = RenderingSystem::TexturePool::LoadTexture("background/levelselect/levelselect");
	bg_title = RenderingSystem::TexturePool::LoadTexture("background/levelselect/title_sepia");

	bg_motif_repeatcount = (RenderingSystem::GetResolution() / bg_motif->GetSize());
	bg_motif_repeatcount.x++; bg_motif_repeatcount.y++;

	soundEmitter = new AudioSystem::AudioSource;
	bgm = AudioSystem::AudioPool::LoadAudio("bgm/youre_my_hero", AudioSystem::OF_AUDIO_TYPE_OGG, true, 96.111f, 6.937f);
	
	ScreenSystem::Screen::LoadContent();
}

void LevelSelectScreen::UnloadContent()
{
	soundEmitter->Stop();
	RenderingSystem::TexturePool::DisposeTexture(bg_motif);
	RenderingSystem::TexturePool::DisposeTexture(bg_title);
	AudioSystem::AudioPool::UnloadAudio(bgm);
	delete soundEmitter;
	delete menuFont;
	ScreenSystem::Screen::UnloadContent();
}

void LevelSelectScreen::Update()
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
			switch (selection) {
			default:
				ScreenSystem::AddScreen(new LevelScreen(selection));
				RemoveMe();
				break;
			case 24: // Exit
				ScreenSystem::AddScreen(new TitleScreen);
				RemoveMe();
				soundEmitter->Stop();
				break;
			}
		}
		break;
	}
	
	if (m_fadetype != 1) return;

	// Change options
	if (m_inputdelay) m_inputdelay--;
	if (InputSystem::GetLeftStick().y > 0.0f && !m_inputdelay) {
		selection++;
		m_inputdelay = 15;
	}
	else if (InputSystem::GetLeftStick().y < 0.0f && !m_inputdelay) {
		selection--;
		m_inputdelay = 15;
	}
	else if (InputSystem::GetLeftStick().x > 0.0f && !m_inputdelay) {
		if (selection < 16) selection += 16;
		else selection -= 16;
		m_inputdelay = 15;
	}
	else if (InputSystem::GetLeftStick().x < 0.0f && !m_inputdelay) {
		if (selection < 16) selection += 16;
		else selection -= 16;
		m_inputdelay = 15;
	}

	// Wrap
	if (selection < 0) selection = maxSelection - 1;
	else if (selection >= maxSelection) selection = 0;


	// Selection
	if (InputSystem::PressedButton(InputSystem::GamePadButton::A))
		//if (selection >= 22 || selection <= 1)
			m_fadetype = 2;

	// Exit
	if (InputSystem::PressedButton(InputSystem::GamePadButton::B)) {
		selection = 24;
		m_fadetype = 2;
	}
}

void LevelSelectScreen::Draw()
{
	vec2 viewportPos = RenderingSystem::GetViewportPosition();
	vec2 viewportSize = RenderingSystem::GetResolution().toVec2();

	//RenderingSystem::glClearColorM(BLUE);

	// Background
	for (dword i = 0u; i < bg_motif_repeatcount.x; i++)
		for (dword j = 0u; j < bg_motif_repeatcount.y; j++)
			bg_motif->Draw(vec2(float(bg_motif->GetSize().x * i), float(bg_motif->GetSize().y * j)), Color4::MaskToColor4(WHITE));
	bg_title->Draw(vec2(viewportSize / 2.0f) - (bg_title->GetSize().toVec2() / 2.0f), Color4(Color4::MaskToColor4(WHITE), 0.7f));

	// Level Select Title
	vec2 textSize = menuFont->MeasureString("* Level Select *", 1.5f);
	menuFont->DrawString(vec2((viewportSize.x / 2.0f) - (textSize.x / 2.0f), 20.0f), "* Level Select *", 1.5f);

	// Levels
	float textYPos = textPosition.y;
	float textXPos = textPosition.x;
	for (int i = 0; i < maxSelection; i++)
	{
		if (i) {
			if (!(i % 2) || i > 21) textYPos += 12.0f;
			if (!(i % 16)) {
				textXPos += (RenderingSystem::GetResolution().x / 4.0f) * 2.0f;
				textYPos = textPosition.y;
			}
			if (i == maxSelection - 1) textYPos += 32.0f;
		}
		menuFont->DrawString(vec2(textXPos, textYPos), levelSelectOptions[i], 1.0f,
			Color4::MaskToColor4((selection == i ? YELLOW : WHITE)), 1.0f);
		textYPos += 8.0f;
	}

	// Fade in
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
