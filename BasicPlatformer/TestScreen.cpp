#include "TestScreen.hpp"
#include <ctime>


TestScreen::TestScreen()
{
	SetActive(true);
	SetVisible(true);
	m_fade = 8.0f;
	m_cameralag = 0;
	m_playerSpindashed = false;
	m_clearcolorset = false;

	// Define current time of day
	time_t now;
	tm* now_tm;
	int hour;
	now = time(nullptr);
	now_tm = localtime(&now);
	hour = now_tm->tm_hour;

	if (hour >= 24) hour -= 24;

	if (hour >= 18)      dayTime = DAYTIME_EVENING;
	else if (hour >= 12) dayTime = DAYTIME_AFTERNOON;
	else if (hour >= 6)  dayTime = DAYTIME_MORNING;
	else                 dayTime = DAYTIME_NIGHT;

	// Define water height
	m_waterHeight = 1800.0f;
}

int count;
TestScreen::~TestScreen()
{
}

void TestScreen::Initialize()
{
	// Ceiling
	/*m_drawables.Add(new Solid(vec2(50.0f, 230.0f), vec2(800.0f, 10.0f), SolidType::RECT));

	m_drawables.Add(new Solid(vec2(0.0f, 350.0f), vec2(300.0f, 250.0f), SolidType::RECT));
	m_drawables.Add(new Solid(vec2(300.0f, 350.0f), vec2(300.0f, 50.0f), SolidType::SLOPE_L));
	m_drawables.Add(new Solid(vec2(300.0f, 400.0f), vec2(300.0f, 200.0f), SolidType::RECT));
	m_drawables.Add(new Solid(vec2(600.0f, 400.0f), vec2(300.0f, 200.0f), SolidType::RECT));
	m_drawables.Add(new Solid(vec2(900.0f, 320.0f), vec2(150.0f, 80.0f), SolidType::SLOPE_R));
	m_drawables.Add(new Solid(vec2(900.0f, 400.0f), vec2(300.0f, 200.0f), SolidType::RECT));
	m_drawables.Add(new Solid(vec2(1200.0f, 320.0f), vec2(150.0f, 80.0f), SolidType::SLOPE_L));
	m_drawables.Add(new Solid(vec2(1200.0f, 400.0f), vec2(300.0f, 200.0f), SolidType::RECT));

	m_drawables.Add(new Solid(vec2(1500.0f, 300.0f), vec2(300.0f, 200.0f), SolidType::RECT));
	*/
	// Small curve
	/*
	m_drawables.Add(new Solid(vec2(1800.0f, 292.0f), vec2(16.0f, 8.0f), SolidType::SLOPE_R));
		m_drawables.Add(new Solid(vec2(1816.0f, 292.0f), vec2(16.0f, 8.0f), SolidType::RECT));
	m_drawables.Add(new Solid(vec2(1816.0f, 276.0f), vec2(16.0f, 16.0f), SolidType::SLOPE_R));
	m_drawables.Add(new Solid(vec2(1832.0f, 260.0f), vec2(8.0f, 16.0f), SolidType::SLOPE_R));
		m_drawables.Add(new Solid(vec2(1832.0f, 276.0f), vec2(8.0f, 24.0f), SolidType::RECT));
	*/
	//m_drawables.Add(new Solid(vec2(1720.0f, 284.0f), vec2(32.0f, 16.0f), SolidType::SLOPE_R));
	//m_drawables.Add(new Solid(vec2(1752.0f, 252.0f), vec2(32.0f, 32.0f), SolidType::SLOPE_R));
	//m_drawables.Add(new Solid(vec2(1784.0f, 220.0f), vec2(16.0f, 32.0f), SolidType::SLOPE_R));

	//m_drawables.Add(new Solid(vec2(1880.0f, 220.0f), vec2(300.0f, 50.0f), SolidType::RECT));
	//m_drawables.Add(new Solid(vec2(1800.0f, 300.0f), vec2(300.0f, 256.0f), SolidType::RECT));
	/*
	// Flying platform
	Solid* platform = new Solid(vec2(2000.0f, 130.0f), vec2(64.0f, 16.0f), SolidType::RECT);
	platform->SetProperty(1, true);
	m_drawables.Add(platform);
	// Another one
	platform = new Solid(vec2(2000.0f, 130.0f), vec2(64.0f, 16.0f), SolidType::RECT);
	platform->SetProperty(1, true);
	m_drawables.Add(platform);

	platform = new Solid(vec2(2300.0f, 200.0f), vec2(64.0f, 16.0f), SolidType::RECT);
	platform->SetProperty(1, true);
	m_drawables.Add(platform);

	platform = new Solid(vec2(2400.0f, 270.0f), vec2(64.0f, 16.0f), SolidType::RECT);
	platform->SetProperty(1, true);
	m_drawables.Add(platform);

	platform = new Solid(vec2(2500.0f, 340.0f), vec2(64.0f, 16.0f), SolidType::RECT);
	platform->SetProperty(1, true);
	m_drawables.Add(platform);

	platform = new Solid(vec2(2600.0f, 410.0f), vec2(64.0f, 16.0f), SolidType::RECT);
	platform->SetProperty(1, true);
	m_drawables.Add(platform);

	platform = new Solid(vec2(2700.0f, 480.0f), vec2(64.0f, 16.0f), SolidType::RECT);
	platform->SetProperty(1, true);
	m_drawables.Add(platform);

	platform = new Solid(vec2(2900.0f, 480.0f), vec2(64.0f, 16.0f), SolidType::RECT);
	platform->SetProperty(1, true);
	m_drawables.Add(platform);

	platform = new Solid(vec2(3100.0f, 480.0f), vec2(64.0f, 16.0f), SolidType::RECT);
	platform->SetProperty(1, true);
	m_drawables.Add(platform);

	platform = new Solid(vec2(3300.0f, 480.0f), vec2(164.0f, 16.0f), SolidType::RECT);
	platform->SetProperty(1, true);
	m_drawables.Add(platform);

	platform = new Solid(vec2(3400.0f, 380.0f), vec2(64.0f, 16.0f), SolidType::RECT);
	platform->SetProperty(1, true);
	m_drawables.Add(platform);

	// The Big Ramp Down
	m_drawables.Add(new Solid(vec2(3500.0f, 380.0f), vec2(200.0f, 200.0f), SolidType::RECT));
	m_drawables.Add(new Solid(vec2(3700.0f, 380.0f), vec2(200.0f, 50.0f), SolidType::SLOPE_L));
	m_drawables.Add(new Solid(vec2(3900.0f, 430.0f), vec2(200.0f, 100.0f), SolidType::SLOPE_L));
	m_drawables.Add(new Solid(vec2(4100.0f, 530.0f), vec2(200.0f, 200.0f), SolidType::SLOPE_L));
	m_drawables.Add(new Solid(vec2(4300.0f, 730.0f), vec2(200.0f, 200.0f), SolidType::SLOPE_L));
	m_drawables.Add(new Solid(vec2(4500.0f, 930.0f), vec2(150.0f, 200.0f), SolidType::SLOPE_L));

	*/
	//m_drawables.Add(new Solid(vec2(0.0f, 1900.0f), vec2(10000.0f, 256.0f), SolidType::RECT));
	// Stress test
	for (float f = 0.0f; f < 992.0f; f += 16.0f)
		m_drawables.Add(new Solid(vec2(f, 384.0f), vec2(16.0f, 16.0f), SolidType::RECT));

	// Stress test
	for (float f = 0.0f; f < 9984.0f; f += 128.0f)
		//for(float v = 16.0f; v < 128.0f; v += 16.0f)
			m_drawables.Add(new Solid(vec2(f, 1808.0f), vec2(128.0f, 112.0f), SolidType::RECT));
	for (float f = 0.0f; f < 9984.0f; f += 256.0f) {
		m_drawables.Add(new Solid(vec2(f, 1792.0f), vec2(32.0f, 16.0f), SolidType::SLOPE_R));
		m_drawables.Add(new Solid(vec2(32.0f + f, 1792.0f), vec2(16.0f, 16.0f), SolidType::RECT));
		m_drawables.Add(new Solid(vec2(48.0f + f, 1792.0f), vec2(32.0f, 16.0f), SolidType::SLOPE_L));
	}
	for (float f = 9984.0f; f < 11264.0f; f += 16.0f)
		m_drawables.Add(new Solid(vec2(f, 1904.0f), vec2(16.0f, 16.0f), SolidType::RECT));

	for (float f = 0.0f; f < 528.0f; f += 16.0f)
		m_drawables.Add(new Solid(vec2(992.0f + f, 384.0f + f), vec2(16.0f, 16.0f), SolidType::SLOPE_L));

	m_grid = new Grid(vec2(128.0f, 128.0f));
	//m_grid->Populate(&m_drawables);

	m_drawables.Add(player = new Player);
	for (auto obj : m_drawables) count++;

	m_drawables.Initialize();


	soundemitter = new OficinaFramework::AudioSystem::AudioSource;

	OficinaFramework::ScreenSystem::Screen::Initialize();
}

void TestScreen::LoadContent()
{
	m_drawables.LoadContent();
	bgm = [&]() {
		switch (dayTime) {
		default:
		case DAYTIME_MORNING:
			return OficinaFramework::AudioSystem::AudioPool::LoadAudio(
				"bgm/mysticcave2p",
				OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG,
				true, 52.679f, 0.612f);
		case DAYTIME_AFTERNOON:
			return OficinaFramework::AudioSystem::AudioPool::LoadAudio(
				"bgm/walkin",
				OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG,
				true, 17.233f, 1.080f);
		case DAYTIME_EVENING:
			return OficinaFramework::AudioSystem::AudioPool::LoadAudio(
				"bgm/hyperhyper",
				OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG,
				true, 41.420f, 11.351f);
		case DAYTIME_NIGHT:
			return OficinaFramework::AudioSystem::AudioPool::LoadAudio(
				"bgm/eveningstar",
				OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG,
				true, 45.096f, 9.097f);
		}
	}();

	soundemitter->Play(bgm);
	player->setGrid(m_grid);
	OficinaFramework::ScreenSystem::Screen::LoadContent();
}

void TestScreen::UnloadContent()
{
	delete m_grid;
	m_drawables.UnloadContent();

	soundemitter->Stop();
	OficinaFramework::AudioSystem::AudioPool::UnloadAudio(bgm);
	delete soundemitter;

	OficinaFramework::ScreenSystem::Screen::UnloadContent();
}

void TestScreen::Update()
{
	// Fullscreen toggle
	if (OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_F11))
	{
		OficinaFramework::ScreenSystem::SetFullScreen(!OficinaFramework::ScreenSystem::IsFullScreen());
		OficinaFramework::RenderingSystem::SetCameraPosition(player->GetPosition());
	}
	// Linear filter toggle
	if (OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_F5)) {
		OficinaFramework::RenderingSystem::SetLinearFiltering(!OficinaFramework::RenderingSystem::GetLinearFilteringState());
	}

	// Camera
	updateCamera();

	// Speed shoes (?)
	if (player->getState() == PLAYER_SPEEDSHOES)
		soundemitter->setPitch(2);
	else soundemitter->setPitch(1);

	// Water height
	player->setWaterHeight(m_waterHeight);

	if (m_fade > -1.0f) { m_fade -= 0.05f;  return; }

	// Debug position reset
	if (OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_F3)) {
		player->reset();
		m_cameralag = 0;
		m_playerSpindashed = false;
		OficinaFramework::RenderingSystem::SetCameraPosition(player->GetPosition());
	}


	m_drawables.Update();
}

void TestScreen::updateCamera()
{
	vec2 cameraPos = OficinaFramework::RenderingSystem::GetCameraPosition();

	// Debug
	if (OficinaFramework::ScreenSystem::IsDebugActive())
	{
		std::stringstream oss;
		oss.clear();
		oss << "CAM >> " << "LAG " << m_cameralag << " " << cameraPos << std::endl
			<< "OBJ >> " << count;
		OficinaFramework::ScreenSystem::Debug_AddLine(oss.str());
	}

	// If the player is spindashing, then set a timer to 32-rev.
	// Whenever it releases, the timer will be set.
	if (player->getAction() == PLAYER_SPINDASH) {
		m_playerSpindashed = true;
		m_cameralag = 16 - player->getSpindashRev();
	}
	else if (m_playerSpindashed) m_playerSpindashed = false;

	// If lagging, don't go beyond this
	if (!m_playerSpindashed && m_cameralag > 0) {
		m_cameralag--;
		return;
	}

	vec2 playerPos = player->GetPosition();
	vec2 resolution = OficinaFramework::RenderingSystem::GetResolution().toVec2();

	// Our ideal viewport
	vec2 desiredViewport = resolution;

	// Calculate camera boundaries
	float cameraBoundsMinX = cameraPos.x - 16.0f;
	float cameraBoundsMaxX = cameraPos.x;
	float cameraBoundsMinYair = cameraPos.y - 48.0f;
	float cameraBoundsMaxYair = cameraPos.y + 16.0f;
	float cameraBoundsY = cameraPos.y - 16.0f;

	// Calculate how much the player exceeds the boundaries, with a maximum of 16.0f so
	// it lags behind on high speed
	vec2 exceed = vec2::Zero();

	// Calculate difference on X axis
	if (playerPos.x >= cameraBoundsMaxX)
		exceed.x = playerPos.x - cameraBoundsMaxX;
	else if (playerPos.x <= cameraBoundsMinX)
		exceed.x = playerPos.x - cameraBoundsMinX;
	// Do not exceed [-16, 16] when following
	clamp(exceed.x, -m_camera_move_speed, m_camera_move_speed);

	// Calculate difference on Y axis
	if (player->OnGround()) {
		if (playerPos.y != cameraBoundsY)
			exceed.y = playerPos.y - cameraBoundsY;
	}
	else {
		if (playerPos.y >= cameraBoundsMaxYair)
			exceed.y = playerPos.y - cameraBoundsMaxYair;
		else if (playerPos.y <= cameraBoundsMinYair)
			exceed.y = playerPos.y - cameraBoundsMinYair;
	}
	// Do not exceed limits when following
	// Limits vary depending on player Y speed
	if (player->Speed().y <= m_camera_move_speed_Yground)
		clamp(exceed.y, -m_camera_move_speed_Yground, m_camera_move_speed_Yground);
	else clamp(exceed.y, -m_camera_move_speed, m_camera_move_speed);

	// Add that difference to our camera position
	//printf("Exceed: %s\n", exceed.toString().c_str());
	cameraPos.x += exceed.x;
	cameraPos.y += exceed.y;

	// Prevent camera from going before the origin(0, 0) on both axes
	if (cameraPos.x <= resolution.x / 2.0f) cameraPos.x = resolution.x / 2.0f;
	if (cameraPos.y <= resolution.y / 2.0f) cameraPos.y = resolution.y / 2.0f;

	// Give the new position to the camera
	OficinaFramework::RenderingSystem::SetCameraPosition(cameraPos);
}

void TestScreen::Draw()
{
	vec2 viewportPos = OficinaFramework::RenderingSystem::GetViewportPosition();
	vec2 viewportSize = OficinaFramework::RenderingSystem::GetViewportSize().toVec2();
	vec2 resolution = OficinaFramework::RenderingSystem::GetResolution().toVec2();

	if (!m_clearcolorset)
	{
		switch (dayTime) {
		default:
		case DAYTIME_MORNING:   OficinaFramework::RenderingSystem::glClearColorM(CORNFLOWERBLUE);  break;
		//case DAYTIME_AFTERNOON: OficinaFramework::RenderingSystem::glClearColorM(ORANGERED);       break;
		//case DAYTIME_EVENING:   OficinaFramework::RenderingSystem::glClearColorM(BLUEVIOLET);      break;
		//case DAYTIME_NIGHT:     OficinaFramework::RenderingSystem::glClearColorM(DARKBLUE);        break;
		}
		m_clearcolorset = true;
	}

	// Drawables
	m_drawables.Draw();

	// Water
	if (m_waterHeight <= viewportPos.y + resolution.y)
	{
		float waterTop = m_waterHeight > viewportPos.y ? m_waterHeight - viewportPos.y : 0.0f;

		glPushMatrix();
		glTranslatef(viewportPos.x, viewportPos.y, 0.0f);
			glBegin(GL_QUADS);
			OficinaFramework::RenderingSystem::glColorM(DARKBLUE, 0.4f);
			glVertex2f(0.0f, waterTop);
			glVertex2f(resolution.x, waterTop);
			glVertex2f(resolution.x, resolution.y);
			glVertex2f(0.0f, resolution.y);
			glEnd();
		glPopMatrix();
	}

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
