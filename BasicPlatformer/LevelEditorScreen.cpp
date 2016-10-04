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

void LevelEditorScreen::restoreResolution()
{
    RenderingSystem::DestroyDefaultBuffer();
    RenderingSystem::SetResolution(m_old_resolution);
    RenderingSystem::CreateDefaultBuffer();
}

void LevelEditorScreen::setResolution()
{
    m_old_resolution = RenderingSystem::GetResolution();

    RenderingSystem::DestroyDefaultBuffer();
    RenderingSystem::SetViewportSize(ScreenSystem::GetWindowSize());
    ScreenSystem::SetWindowSize(RenderingSystem::GetViewportSize());
    RenderingSystem::SetResolution(RenderingSystem::GetViewportSize());
    RenderingSystem::CreateDefaultBuffer();
}

void LevelEditorScreen::Update()
{
    if(m_old_resolution == 0u)
    {
        // First run
        setResolution();
        RenderingSystem::glClearColorM(CORNFLOWERBLUE);
        // Disable debug
        ScreenSystem::SetDebug(false);
    }

    RenderingSystem::SetCameraPosition(m_cameraPosition);
    m_cameraPosition += InputSystem::GetLeftStick();


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
        ImGui::Text("Viewport: %s\nResolution:%s\n",
            RenderingSystem::GetViewportSize().toVec2().toString().c_str(),
            RenderingSystem::GetResolution().toVec2().toString().c_str());
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
                restoreResolution();
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
        if(ImGui::BeginMenu("Window"))
        {
            if(ImGui::BeginMenu("Theme..."))
            {
                ImGui::RadioButton("Dark", &theme, 0);
                ImGui::RadioButton("OSX",  &theme, 1);
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Process events
    ImGui_ImplSdl_ProcessEvents();

    updateTheme();
}

void LevelEditorScreen::updateTheme()
{
    if(currentTheme != theme)
    {
        currentTheme = theme;
        switch(theme)
        {
        case 1: // White theme
            // OSX theme by itamago
            ImGui::GetStyle().Colors[ImGuiCol_Text]                  = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]          = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
            //ImGui::GetStyle().Colors[ImGuiCol_TextHovered]           = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            //ImGui::GetStyle().Colors[ImGuiCol_TextActive]            = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
            ImGui::GetStyle().Colors[ImGuiCol_WindowBg]              = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
            ImGui::GetStyle().Colors[ImGuiCol_ChildWindowBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            ImGui::GetStyle().Colors[ImGuiCol_Border]                = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
            ImGui::GetStyle().Colors[ImGuiCol_BorderShadow]          = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
            ImGui::GetStyle().Colors[ImGuiCol_FrameBg]               = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
            ImGui::GetStyle().Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
            ImGui::GetStyle().Colors[ImGuiCol_TitleBg]               = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
            ImGui::GetStyle().Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
            ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
            ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
            ImGui::GetStyle().Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
            ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
            ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
            ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
            ImGui::GetStyle().Colors[ImGuiCol_ComboBg]               = ImVec4(0.86f, 0.86f, 0.86f, 0.99f);
            ImGui::GetStyle().Colors[ImGuiCol_CheckMark]             = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            ImGui::GetStyle().Colors[ImGuiCol_SliderGrab]            = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
            ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            ImGui::GetStyle().Colors[ImGuiCol_Button]                = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
            ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]          = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
            ImGui::GetStyle().Colors[ImGuiCol_Header]                = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
            ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
            ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            ImGui::GetStyle().Colors[ImGuiCol_Column]                = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
            ImGui::GetStyle().Colors[ImGuiCol_ColumnHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
            ImGui::GetStyle().Colors[ImGuiCol_ColumnActive]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            ImGui::GetStyle().Colors[ImGuiCol_ResizeGrip]            = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
            ImGui::GetStyle().Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
            ImGui::GetStyle().Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
            ImGui::GetStyle().Colors[ImGuiCol_CloseButton]           = ImVec4(0.59f, 0.59f, 0.59f, 0.50f);
            ImGui::GetStyle().Colors[ImGuiCol_CloseButtonHovered]    = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
            ImGui::GetStyle().Colors[ImGuiCol_CloseButtonActive]     = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
            ImGui::GetStyle().Colors[ImGuiCol_PlotLines]             = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
            ImGui::GetStyle().Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
            ImGui::GetStyle().Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
            ImGui::GetStyle().Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
            ImGui::GetStyle().Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
            ImGui::GetStyle().Colors[ImGuiCol_PopupBg]               = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
            ImGui::GetStyle().Colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
            break;
        default: break;
        }
    }
}

void LevelEditorScreen::drawGrid()
{
    vec2 resolution = RenderingSystem::GetResolution().toVec2();
    vec2 vwprtPos = RenderingSystem::GetViewportPosition();
    vec2 start = vec2::Zero();

    glPushMatrix();
    //glTranslatef(vwprtPos.x, vwprtPos.y, 0.0f);
    for(float i = start.x; i < resolution.x; i += 128.0f)
    {
        RenderingSystem::glColorM(BLACK);
        glBegin(GL_LINES);
            glVertex2f(i, 0.0f);
            glVertex2f(i, resolution.y);
        glEnd();
    }

    for(float j = start.y; j < resolution.y; j += 128.0f)
    {
        RenderingSystem::glColorM(BLACK);
        glBegin(GL_LINES);
            glVertex2f(0.0f, j);
            glVertex2f(resolution.x, j);
        glEnd();
    }
    glPopMatrix();
}

void LevelEditorScreen::Draw()
{
    drawGrid();

    glPushMatrix();
    ImGui::Render();
    glPopMatrix();
}