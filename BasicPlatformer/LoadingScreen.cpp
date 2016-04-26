#include "LoadingScreen.hpp"
#include <OficinaFramework\InputSystem.hpp>
#include <OficinaFramework\TimingSystem.hpp>

/* Loading Screen */
void LoadingScreen::Initialize() {
	SetActive(true);
	SetVisible(true);
	ScreenSystem::Screen::Initialize();
}

void LoadingScreen::LoadContent() {
	font = new RenderingSystem::Font(RenderingSystem::TexturePool::LoadTexture("fonts/levelselect"), vec2dw(8u), vec2b::One());
	ScreenSystem::Screen::LoadContent();
}

void LoadingScreen::UnloadContent() {
	delete font;
	ScreenSystem::Screen::UnloadContent();
}

void LoadingScreen::Update() {
	if (!ScreenSystem::IsLoadingThreadBusy()) return;

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

	angle += 20.0f * TimingSystem::StepCorrection();
	if (angle > 360.0f) angle -= 360.0f;

	if (InputSystem::PressingKey(SDL_SCANCODE_PAGEUP))
		RenderingSystem::SetZoomFactor(RenderingSystem::GetZoomFactor() + (0.01f * TimingSystem::StepCorrection()));
	else if (InputSystem::PressingKey(SDL_SCANCODE_PAGEDOWN))
		RenderingSystem::SetZoomFactor(RenderingSystem::GetZoomFactor() - (0.01f * TimingSystem::StepCorrection()));
	else if (InputSystem::PressedKey(SDL_SCANCODE_HOME))
		RenderingSystem::SetZoomFactor(1.0f);
	RenderingSystem::SetCameraPosition(vec2::Zero());
}

void LoadingScreen::Draw() {

	if (!ScreenSystem::IsLoadingThreadBusy()) return;

	vec2 viewportPos = RenderingSystem::GetViewportPosition();
	vec2 viewportSize = RenderingSystem::GetResolution().toVec2();

	glPushMatrix();
	glTranslatef(viewportPos.x, viewportPos.y, 0.0f);
	OficinaFramework::RenderingSystem::glColorM(BLACK, 1.0f);
	glBegin(GL_QUADS);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(viewportSize.x, 0.0f);
	glVertex2f(viewportSize.x, viewportSize.y);
	glVertex2f(0.0f, viewportSize.y);
	glEnd();
	glPopMatrix();

	vec2 modelpos = RenderingSystem::GetResolution().toVec2() / 2.0f;
	float transparency = 0.5f;
	glPushMatrix();
	glTranslatef(RenderingSystem::GetViewportPosition().x, RenderingSystem::GetViewportPosition().y, 0.0f);

	glPushMatrix();
	RenderingSystem::glColorM(ORANGE);
	glTranslatef(modelpos.x, modelpos.y, 100.0f);
	glRotatef(angle, 0.5f, 1.0f, -0.5f);
	//glScalef(0.5f, 0.5f, 0.5f);

	glBegin(GL_POLYGON);
	glColor4f(1.0f, 0.0f, 0.0f, transparency);
	glVertex3f(50.0f, -50.0f, -50.0f);      // P1 is red
	glColor4f(0.0f, 1.0f, 0.0f, transparency);
	glVertex3f(50.0f, 50.0f, -50.0f);      // P2 is green
	glColor4f(0.0f, 0.0f, 1.0f, transparency);
	glVertex3f(-50.0f, 50.0f, -50.0f);      // P3 is blue
	glColor4f(1.0F, 0.0F, 1.0F, transparency);
	glVertex3f(-50.0f, -50.0f, -50.0f);      // P4 is purple
	glEnd();
	// White side - BACK
	glBegin(GL_POLYGON);
	glColor4f(0.4f, 0.4f, 0.4f, transparency);
	glVertex3f(50.0f, -50.0f, 50.0f);
	glVertex3f(50.0f, 50.0f, 50.0f);
	glVertex3f(-50.0f, 50.0f, 50.0f);
	glVertex3f(-50.0f, -50.0f, 50.0f);
	glEnd();

	// Purple side - RIGHT
	glBegin(GL_POLYGON);
	glColor4f(1.0f, 0.0f, 1.0f, transparency);
	glVertex3f(50.0f, -50.0f, -50.0f);
	glVertex3f(50.0f, 50.0f, -50.0f);
	glVertex3f(50.0f, 50.0f, 50.0f);
	glVertex3f(50.0f, -50.0f, 50.0f);
	glEnd();

	// Green side - LEFT
	glBegin(GL_POLYGON);
	glColor4f(0.0f, 1.0f, 0.0f, transparency);
	glVertex3f(-50.0f, -50.0f, 50.0f);
	glVertex3f(-50.0f, 50.0f, 50.0f);
	glVertex3f(-50.0f, 50.0f, -50.0f);
	glVertex3f(-50.0f, -50.0f, -50.0f);
	glEnd();

	// Blue side - TOP
	glBegin(GL_POLYGON);
	glColor4f(0.0f, 0.0f, 1.0f, transparency);
	glVertex3f(50.0f, 50.0f, 50.0f);
	glVertex3f(50.0f, 50.0f, -50.0f);
	glVertex3f(-50.0f, 50.0f, -50.0f);
	glVertex3f(-50.0f, 50.0f, 50.0f);
	glEnd();

	// Red side - BOTTOM
	glBegin(GL_POLYGON);
	glColor4f(1.0f, 0.0f, 0.0f, transparency);
	glVertex3f(50.0f, -50.0f, -50.0f);
	glVertex3f(50.0f, -50.0f, 50.0f);
	glVertex3f(-50.0f, -50.0f, 50.0f);
	glVertex3f(-50.0f, -50.0f, -50.0f);
	glEnd();
	glPopMatrix();

	vec2 fontsize = font->MeasureString("NOW LOADING", 1.0f);
	font->DrawString(vec2(modelpos.x - (fontsize.x / 2.0f), modelpos.y), "NOW LOADING", 1.0f, Color4::MaskToColor4(WHITE), transparency);

	glPopMatrix();
}
