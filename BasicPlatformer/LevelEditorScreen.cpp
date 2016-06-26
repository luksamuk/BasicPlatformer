#include "LevelEditorScreen.hpp"
#include "TitleScreen.hpp"
#include <OficinaFramework/InputSystem.hpp>
using namespace OficinaFramework;

LevelEditorScreen::LevelEditorScreen()
{
    SetActive(true);
    SetVisible(true);
}

void LevelEditorScreen::Initialize()
{
    // Start ImGui
    ImGui_ImplSdl_Init(ScreenSystem::GetWindowHandle());
    clear_color = ImColor(114, 144, 154);

    ScreenSystem::Screen::Initialize();
}

void LevelEditorScreen::LoadContent()
{
    ScreenSystem::Screen::LoadContent();
}

void LevelEditorScreen::UnloadContent()
{
    ImGui_ImplSdl_Shutdown();
    ScreenSystem::Screen::UnloadContent();
}

void LevelEditorScreen::Update()
{
    RenderingSystem::SetCameraPosition(RenderingSystem::GetResolution().toVec2() / 2.0f);

    // Debug toggle
	if (InputSystem::PressedKey(SDL_SCANCODE_F1)
		// Lstick + RB
		|| (InputSystem::PressingButton(InputSystem::GamePadButton::LSTICK)
			&& InputSystem::PressedButton(InputSystem::GamePadButton::RSHOULDER1)))
		ScreenSystem::SetDebug(!ScreenSystem::IsDebugActive());
	if (InputSystem::PressedKey(SDL_SCANCODE_F2)
		|| InputSystem::PressedButton(InputSystem::GamePadButton::LSHOULDER1))
		ScreenSystem::Debug_ToggleMinimalist();

	// Fullscreen toggle
	if (OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_F11))
		OficinaFramework::ScreenSystem::SetFullScreen(!OficinaFramework::ScreenSystem::IsFullScreen());

    // TODO: Make ImGui process input events (typing-related)
    // or write an event handler for ImGui myself
    ImGui_ImplSdl_NewFrame(ScreenSystem::GetWindowHandle());

    {
        static float f = 0.0f;
        ImGui::Text("Hello, world!");
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&clear_color);
        if (ImGui::Button("Test Window")) show_test_window ^= 1;
        if (ImGui::Button("Another Window")) show_another_window ^= 1;
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    // 2. Show another simple window, this time using an explicit Begin/End pair
    if (show_another_window)
    {
        ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Another Window", &show_another_window);
        ImGui::Text("Hello");
        ImGui::End();
    }

    // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
    if (show_test_window)
    {
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
        ImGui::ShowTestWindow(&show_test_window);
    }

    // Main menu
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Project...")) {}
            if (ImGui::MenuItem("Load Project...")) {}
            if (ImGui::MenuItem("Load Project...")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Save Project")) {}
            if (ImGui::MenuItem("Save Project As...")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Export...")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Back to Main Menu"))
            {
                ScreenSystem::AddScreen(new TitleScreen);
                RemoveMe();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void LevelEditorScreen::Draw()
{
    glPushMatrix();
    ImGui::Render();
    glPopMatrix();
}