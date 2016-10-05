#include "EditorScreen.hpp"
#include <OficinaFramework/InputSystem.hpp>
#include "imgui_impl_sdl.hpp"
using namespace OficinaFramework;

EditorScreen::EditorScreen()
{
    SetActive(true);
    SetVisible(true);
}

void EditorScreen::Initialize()
{
    // Start ImGui
    ImGui_ImplSdl_Init(ScreenSystem::GetWindowHandle());
    clear_color = ImColor(114, 144, 154);
    //m_cameraPosition = CAMERA_MIN_VEC;
    m_levelSize = vec2(4000);
    ScreenSystem::Screen::Initialize();
}

void EditorScreen::LoadContent()
{
    ScreenSystem::Screen::LoadContent();
}

void EditorScreen::UnloadContent()
{
    ImGui_ImplSdl_Shutdown();
    ScreenSystem::Screen::UnloadContent();
}

void EditorScreen::Update()
{
    // Get Left Stick
    vec2 lstick = InputSystem::GetLeftStick();

    // Accelerate camera
    m_cameraSpeed += lstick * 0.2f;
    m_cameraSpeed.clamp(vec2(-20.0f), vec2(20.0f));
    // Decelerate camera
    if (lstick.x == 0.0f) {
        if((abs(m_cameraSpeed.x) > 0.2f))
            m_cameraSpeed.x -= 0.4f * sign(m_cameraSpeed.x);
        else m_cameraSpeed.x = 0.0f;
    }
    if (lstick.y == 0.0f) {
        if((abs(m_cameraSpeed.y) > 0.2f))
            m_cameraSpeed.y -= 0.4f * sign(m_cameraSpeed.y);
        else m_cameraSpeed.y = 0.0f;
    }
    // Limit camera
    m_cameraPosition.clamp(CAMERA_MIN_VEC, CAMERA_MAX_VEC);

    // Stop camera when hitting boundaries
    if((m_cameraSpeed.x < 0.0f && m_cameraPosition.x == CAMERA_MIN_X)
        || (m_cameraSpeed.x > 0.0f && m_cameraPosition.x == CAMERA_MAX_X))
        m_cameraSpeed.x = 0.0f;
    if((m_cameraSpeed.y < 0.0f && m_cameraPosition.y == CAMERA_MIN_Y)
        || (m_cameraSpeed.y > 0.0f && m_cameraPosition.y == CAMERA_MAX_Y))
        m_cameraSpeed.y = 0.0f;

    // Transform camera
    m_cameraPosition += m_cameraSpeed;
    RenderingSystem::SetCameraPosition(m_cameraPosition);


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
        ImGui::Text("Viewport: %s\nResolution:%s\nVwpPos: %s\nCamSpd: %s\nlstick: %s\n",
            RenderingSystem::GetViewportSize().toVec2().toString().c_str(),
            RenderingSystem::GetResolution().toVec2().toString().c_str(),
            RenderingSystem::GetViewportPosition().toString().c_str(),
            m_cameraSpeed.toString().c_str(),
            lstick.toString().c_str());
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
            if (ImGui::MenuItem("Quit"))
            {
                //restoreResolution();
                //ScreenSystem::AddScreen(new TitleScreen);
                //RemoveMe();
                InputSystem::CallExitCommand();
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
    //ImGui_ImplSdl_ProcessEvents();

    updateTheme();
}

void EditorScreen::updateTheme()
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

void EditorScreen::drawGrid()
{
    vec2 resolution = RenderingSystem::GetResolution().toVec2();
    vec2 vwprtPos = RenderingSystem::GetViewportPosition();
    vec2 start = vec2::Zero();
    start.x = -(vwprtPos.x - (((int)(vwprtPos.x / 128) * 128.0f)));
    start.y = -(vwprtPos.y - (((int)(vwprtPos.y / 128) * 128.0f)));

    glPushMatrix();
    glTranslatef(vwprtPos.x, vwprtPos.y, 0.0f);
    for(float i = start.x; i < resolution.x; i += 128.0f)
    {
        RenderingSystem::glColorM(BLACK, 0.2f);
        glBegin(GL_LINES);
            glVertex2f(i, 0.0f);
            glVertex2f(i, resolution.y);
        glEnd();
    }

    for(float j = start.y; j < resolution.y; j += 128.0f)
    {
        RenderingSystem::glColorM(BLACK, 0.2f);
        glBegin(GL_LINES);
            glVertex2f(0.0f, j);
            glVertex2f(resolution.x, j);
        glEnd();
    }
    glPopMatrix();

    // Scroll Bar
    glPushMatrix();
    glTranslatef(vwprtPos.x, vwprtPos.y, 0.0f);

    // Vertical Scroll Bar
    {
        // Useful area
        float usefulAreaSize = resolution.y - SCROLLBAR_WIDTH;
        // Compensate for menu bar
        usefulAreaSize -= 20.0f;
        glPushMatrix();
        // Translate to right side of screen
        glTranslatef(resolution.x - SCROLLBAR_WIDTH, 20.0f, 0.0f);
        glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
        glBegin(GL_QUADS);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(SCROLLBAR_WIDTH, 0.0f);
        glVertex2f(SCROLLBAR_WIDTH, usefulAreaSize);
        glVertex2f(0.0f, usefulAreaSize);
        glEnd();

        // Draw scroller
        glPushMatrix();
        {
            float scrollerSize = usefulAreaSize / (m_levelSize.y / resolution.y);
            float scrollerPosFactor = (vwprtPos.y / (m_levelSize.y - resolution.y));
            float scrollerPosition = scrollerPosFactor * (usefulAreaSize - scrollerSize);
            glTranslatef(0.0f, scrollerPosition, 0.0f);
            glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
            glBegin(GL_QUADS);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(SCROLLBAR_WIDTH, 0.0f);
            glVertex2f(SCROLLBAR_WIDTH, scrollerSize);
            glVertex2f(0.0f, scrollerSize);
            glEnd();
        }
        glPopMatrix();

        glPopMatrix();
    }

    // Horizontal Scroll Bar
    {
        // Useful area
        float usefulAreaSize = resolution.x - SCROLLBAR_WIDTH;
        glPushMatrix();
        // Translate to bottom side of screen
        glTranslatef(0.0f, resolution.y - SCROLLBAR_WIDTH, 0.0f);
        glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
        glBegin(GL_QUADS);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(0.0f, SCROLLBAR_WIDTH);
        glVertex2f(usefulAreaSize, SCROLLBAR_WIDTH);
        glVertex2f(usefulAreaSize, 0.0f);
        glEnd();

        // Draw scroller
        glPushMatrix();
        {
            float scrollerSize = usefulAreaSize / (m_levelSize.x / resolution.x);
            float scrollerPosFactor = (vwprtPos.x / (m_levelSize.x - resolution.x));
            float scrollerPosition = scrollerPosFactor * (usefulAreaSize - scrollerSize);

            glTranslatef(scrollerPosition, 0.0f, 0.0f);
            glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
            glBegin(GL_QUADS);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(0.0f, SCROLLBAR_WIDTH);
            glVertex2f(scrollerSize, SCROLLBAR_WIDTH);
            glVertex2f(scrollerSize, 0.0f);
            glEnd();
        }
        glPopMatrix();

        glPopMatrix();
    }
    glPopMatrix();
}

void EditorScreen::Draw()
{
    drawGrid();

    glPushMatrix();
    ImGui::Render();
    glPopMatrix();
}