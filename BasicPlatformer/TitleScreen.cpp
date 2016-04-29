#include "TitleScreen.hpp"
#include "TestScreen.hpp"
#include <OficinaFramework\InputSystem.hpp>
using namespace OficinaFramework;

TitleScreen::TitleScreen()
{
	SetVisible(true);
	SetActive(true);
}

void TitleScreen::Initialize()
{
	textPosition = vec2(20.0f, 40.0f);
	selection = 0;
	ScreenSystem::Screen::Initialize();
}

void TitleScreen::LoadContent()
{
	menuFont = new RenderingSystem::Font(RenderingSystem::TexturePool::LoadTexture("fonts/levelselect"),
		vec2dw(8u), vec2b::One());
	bg_motif = RenderingSystem::TexturePool::LoadTexture("background/levelselect");

	bg_motif_repeatcount = (RenderingSystem::GetResolution() / bg_motif->GetSize());
	bg_motif_repeatcount.x++; bg_motif_repeatcount.y++;

	soundEmitter = new AudioSystem::AudioSource;
	bgm = AudioSystem::AudioPool::LoadAudio("bgm/youre_my_hero", AudioSystem::OF_AUDIO_TYPE_OGG, true, 96.111f, 6.937f);
	
	ScreenSystem::Screen::LoadContent();
}

void TitleScreen::UnloadContent()
{
	soundEmitter->Stop();
	RenderingSystem::TexturePool::DisposeTexture(bg_motif);
	AudioSystem::AudioPool::UnloadAudio(bgm);
	delete soundEmitter;
	delete menuFont;
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
				ScreenSystem::AddScreen(new TestScreen);
				break;
			case 14: // Exit
				InputSystem::CallExitCommand();
				break;
			}
			RemoveMe();
		}
		break;
	}
	
	if (m_fadetype != 1) return;

	// Change options
	if (InputSystem::MovedStick(InputSystem::ThumbStick::LEFTSTICK,
		InputSystem::ThumbStickAxis::VERTICAL,
		InputSystem::ThumbStickAxisSignal::POSITIVE))
		selection++;
	else if (InputSystem::MovedStick(InputSystem::ThumbStick::LEFTSTICK,
		InputSystem::ThumbStickAxis::VERTICAL,
		InputSystem::ThumbStickAxisSignal::NEGATIVE))
		selection--;

	// Limits
	if (selection < 0) selection = 0;
	else if (selection >= maxSelection) selection = maxSelection - 1;


	// Selection
	if (InputSystem::PressedButton(InputSystem::GamePadButton::A))
		if (selection == 0 || selection == maxSelection - 1)
			m_fadetype = 2;
}

void TitleScreen::Draw()
{
	vec2 viewportPos = RenderingSystem::GetViewportPosition();
	vec2 viewportSize = RenderingSystem::GetResolution().toVec2();

	//RenderingSystem::glClearColorM(BLUE);

	// Background
	for (dword i = 0u; i < bg_motif_repeatcount.x; i++)
		for (dword j = 0u; j < bg_motif_repeatcount.y; j++)
			bg_motif->Draw(vec2(float(bg_motif->GetSize().x * i), float(bg_motif->GetSize().y * j)), Color4::MaskToColor4(ColorDef::WHITE));

	// Level Select Title
	vec2 textSize = menuFont->MeasureString("* Level Select *", 1.0f);
	menuFont->DrawString(vec2((viewportSize.x / 2.0f) - (textSize.x / 2.0f), 20.0f), "* Level Select *");

	// Levels
	float textYPos = textPosition.y;
	for (int i = 0; i < maxSelection; i++)
	{
		if (i) {
			if (!(i % 2)) textYPos += 16.0f;
			//if (!(i % 19)) {
			//	textXPos += 224.0f;
			//	textYpos = textPosition.y;
			//}
		}
		menuFont->DrawString(vec2(textPosition.x, textYPos), levelSelectOptions[i], 1.0f,
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
