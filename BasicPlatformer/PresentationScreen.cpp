#include "PresentationScreen.hpp"
#include "TitleScreen.hpp"
#include <OficinaFramework\TimingSystem.hpp>
#include <OficinaFramework\InputSystem.hpp>

/* Presentation Screen */
void PresentationScreen::Initialize() {
	SetActive(true);
	SetVisible(true);
	m_poweredby_texture = NULL;
	m_poweredby_sheet = NULL;
	m_poweredby = NULL;
	m_timer = 0.0f;
	ScreenSystem::Screen::Initialize();
}
void PresentationScreen::LoadContent() {
	m_poweredby_texture = RenderingSystem::TexturePool::LoadTexture("sprites/poweredby");
	m_poweredby_sheet = new RenderingSystem::SpriteSheet(vec2dw(640u, 480u), vec2b::Zero(), vec2::Zero());
	m_poweredby_sheet->AppendTexture(m_poweredby_texture);
	m_poweredby = new RenderingSystem::Animation(m_poweredby_sheet);
	m_poweredby->RegisterAnimation("FadeIn", 0, 7, 7, 0.03f);
	m_poweredby->RegisterAnimation("Idle", 7, 7, 7, 1.0f);
	m_poweredby->RegisterAnimation("FadeOut", 8, 15, 15, 0.03f);
	m_poweredby->SetAnimation("FadeIn");
	ScreenSystem::Screen::LoadContent();
}
void PresentationScreen::UnloadContent() {
	delete m_poweredby;
	delete m_poweredby_sheet;
	RenderingSystem::TexturePool::DisposeTexture(m_poweredby_texture);
	ScreenSystem::Screen::UnloadContent();
}
void PresentationScreen::Update() {
	m_timer += 0.02f * TimingSystem::StepCorrection();
	m_poweredby->update();

	if (m_timer >= 1.0f && m_timer < 5.0f
		&& InputSystem::PressedButton(InputSystem::GamePadButton::A)) {
		m_timer = 5.0f;
	}
	else if (m_timer >= 6.0f) {
		ScreenSystem::AddScreen(new TitleScreen);
		RemoveMe();
	}
	if (m_timer >= 5.0f && m_poweredby->GetCurrentAnimationName() != "FadeOut")
		m_poweredby->SetAnimation("FadeOut");


	// Debug toggle
	if (InputSystem::PressedKey(SDL_SCANCODE_F1))
		ScreenSystem::SetDebug(!ScreenSystem::IsDebugActive());
	if (InputSystem::PressedKey(SDL_SCANCODE_F2))
		ScreenSystem::Debug_ToggleMinimalist();

	// Fullscreen toggle
	if (OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_F11))
		OficinaFramework::ScreenSystem::SetFullScreen(!OficinaFramework::ScreenSystem::IsFullScreen());
	// Linear filter toggle
	if (OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_F5))
		OficinaFramework::RenderingSystem::SetLinearFiltering(!OficinaFramework::RenderingSystem::GetLinearFilteringState());
}
void PresentationScreen::Draw() {
	// Black background
	glPushMatrix();
	glTranslatef(RenderingSystem::GetViewportPosition().x, RenderingSystem::GetViewportPosition().y, 0.0f);
	RenderingSystem::glColorM(BLACK, 1.0f);
	glBegin(GL_QUADS);
	glVertex2f(0.0f, 0.0);
	glVertex2f(RenderingSystem::GetResolution().toVec2().x, 0.0f);
	glVertex2f(RenderingSystem::GetResolution().toVec2().x,
		RenderingSystem::GetResolution().toVec2().y);
	glVertex2f(0.0f, RenderingSystem::GetResolution().toVec2().y);
	glEnd();

	m_poweredby->Draw(RenderingSystem::GetResolution().toVec2() / 2.0f, 1.0f, Color4::MaskToColor4(WHITE));
	glPopMatrix();
}