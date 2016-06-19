#include "LevelScreen.hpp"
#include "LevelSelectScreen.hpp"
#include <ctime>
#include <sstream>
using namespace OficinaFramework;


LevelScreen::LevelScreen(dword id)
{
	SetActive(true);
	SetVisible(true);
	m_fade = 8.0f;
	m_cameralag = 0;
	m_playerSpindashed = false;
	m_clearcolorset = false;
	LEVEL_ID = id;

	// Title Card
	m_titlecard_pos = RenderingSystem::GetResolution().toVec2();
	m_titlecard_pos.y /= 3.0f;
	switch (id)
	{
	case 0:
		LEVEL_NAME = "ISOLATED ISLAND";
		LEVEL_ACT = 1;
		break;
	case 1:
		LEVEL_NAME = "ISOLATED ISLAND";
		LEVEL_ACT = 2;
		break;
	case 2:
		LEVEL_NAME = "AMAZING AQUARIUM";
		LEVEL_ACT = 1;
		break;
	case 3:
		LEVEL_NAME = "AMAZING AQUARIUM";
		LEVEL_ACT = 2;
		break;
	case 4:
		LEVEL_NAME = "FREEZING FACTORY";
		LEVEL_ACT = 1;
		break;
	case 5:
		LEVEL_NAME = "FREEZING FACTORY";
		LEVEL_ACT = 2;
		break;
    case 6:
		LEVEL_NAME = "HAZEL HILL";
		LEVEL_ACT = 1;
		break;
	case 7:
		LEVEL_NAME = "HAZEL HILL";
		LEVEL_ACT = 2;
		break;
	case 8:
		LEVEL_NAME = "DUSTY DESERT";
		LEVEL_ACT = 1;
		break;
	case 9:
		LEVEL_NAME = "DUSTY DESERT";
		LEVEL_ACT = 2;
		break;
	case 10:
		LEVEL_NAME = "JESTER JUGGLE";
		LEVEL_ACT = 1;
		break;
	case 11:
		LEVEL_NAME = "JESTER JUGGLE";
		LEVEL_ACT = 2;
		break;
	case 12:
		LEVEL_NAME = "CHAOTIC CANYON";
		LEVEL_ACT = 1;
		break;
	case 13:
		LEVEL_NAME = "CHAOTIC CANYON";
		LEVEL_ACT = 2;
		break;
	case 14:
		LEVEL_NAME = "RADIANT RUSH";
		LEVEL_ACT = 1;
		break;
	case 15:
		LEVEL_NAME = "RADIANT RUSH";
		LEVEL_ACT = 2;
		break;
	case 16:
		LEVEL_NAME = "CYBER CITY";
		LEVEL_ACT = 1;
		break;
	case 17:
		LEVEL_NAME = "CYBER CITY";
		LEVEL_ACT = 2;
		break;
	case 18:
		LEVEL_NAME = "WHITE WONDER";
		LEVEL_ACT = 0;
		break;
	case 19:
		LEVEL_NAME = "STORMY SKY";
		LEVEL_ACT = 0;
		break;
	case 20:
		LEVEL_NAME = "COSMIC COLONY";
		LEVEL_ACT = 1;
		break;
	case 21:
		LEVEL_NAME = "COSMIC COLONY";
		LEVEL_ACT = 2;
		break;
	case 22:
		LEVEL_NAME = "THE FINAL";
		LEVEL_ACT = 0;
		break;
	case 23:
		LEVEL_NAME = "ENGINE TEST";
		LEVEL_ACT = 0;
		break;
	case 25:
		LEVEL_NAME = "LOST HOPE";
		LEVEL_ACT = 0;
		break;
	default:
		LEVEL_NAME = "?????";
		LEVEL_ACT = 0;
		break;
	}

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
	m_hasWater = true;
	m_waterHeight = 1864.0f;

	if (LEVEL_ID == 22u || LEVEL_ID == 19u || LEVEL_ID == 12u || LEVEL_ID == 13u
		|| LEVEL_ID == 8u || LEVEL_ID == 9u || LEVEL_ID == 20u || LEVEL_ID == 21u)
		m_hasWater = false;
}

LevelScreen::~LevelScreen()
{
}

void LevelScreen::Initialize()
{
	if (LEVEL_ID == 0 || LEVEL_ID == 1)
	{
		// Ceiling
		//m_drawables.Add(new Solid(vec2(50.0f, 230.0f), vec2(800.0f, 10.0f), SolidType::RECT));

		m_drawables.Add(new Solid(vec2(0.0f, 350.0f), vec2(300.0f, 250.0f), SolidType::RECT));
		m_drawables.Add(new Solid(vec2(300.0f, 350.0f), vec2(300.0f, 50.0f), SolidType::SLOPE_L));
		m_drawables.Add(new Solid(vec2(300.0f, 400.0f), vec2(300.0f, 200.0f), SolidType::RECT));
		m_drawables.Add(new Solid(vec2(600.0f, 400.0f), vec2(300.0f, 200.0f), SolidType::RECT));
		m_drawables.Add(new Solid(vec2(900.0f, 320.0f), vec2(150.0f, 80.0f), SolidType::SLOPE_R));
		m_drawables.Add(new Solid(vec2(900.0f, 400.0f), vec2(300.0f, 200.0f), SolidType::RECT));
		m_drawables.Add(new Solid(vec2(1200.0f, 320.0f), vec2(150.0f, 80.0f), SolidType::SLOPE_L));
		m_drawables.Add(new Solid(vec2(1200.0f, 400.0f), vec2(300.0f, 200.0f), SolidType::RECT));

		m_drawables.Add(new Solid(vec2(1500.0f, 300.0f), vec2(300.0f, 200.0f), SolidType::RECT));
		
		// Small curve
		
		/*m_drawables.Add(new Solid(vec2(1800.0f, 292.0f), vec2(16.0f, 8.0f), SolidType::SLOPE_R));
		m_drawables.Add(new Solid(vec2(1816.0f, 292.0f), vec2(16.0f, 8.0f), SolidType::RECT));
		m_drawables.Add(new Solid(vec2(1816.0f, 276.0f), vec2(16.0f, 16.0f), SolidType::SLOPE_R));
		m_drawables.Add(new Solid(vec2(1832.0f, 260.0f), vec2(8.0f, 16.0f), SolidType::SLOPE_R));
		m_drawables.Add(new Solid(vec2(1832.0f, 276.0f), vec2(8.0f, 24.0f), SolidType::RECT));*/
		
		m_drawables.Add(new Solid(vec2(1720.0f, 284.0f), vec2(32.0f, 16.0f), SolidType::SLOPE_R));
		m_drawables.Add(new Solid(vec2(1752.0f, 252.0f), vec2(32.0f, 32.0f), SolidType::SLOPE_R));
		m_drawables.Add(new Solid(vec2(1784.0f, 220.0f), vec2(16.0f, 32.0f), SolidType::SLOPE_R));

		m_drawables.Add(new Solid(vec2(1880.0f, 220.0f), vec2(300.0f, 50.0f), SolidType::RECT));
		m_drawables.Add(new Solid(vec2(1800.0f, 300.0f), vec2(300.0f, 256.0f), SolidType::RECT));
		
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

		for (float f = 0.0f; f < 6400.0f; f += 128.0f)
			m_drawables.Add(new Solid(vec2(f, 1776.0f), vec2(128.0f, 128.0f), SolidType::RECT));
	}
	else if(LEVEL_ID == 22u || LEVEL_ID == 23u)
	{
		// Stress test
		for (float f = 0.0f; f < 992.0f; f += 16.0f)
			m_drawables.Add(new Solid(vec2(f, 368.0f), vec2(16.0f, 16.0f), SolidType::RECT));

		// Stress test
		for (float f = 0.0f; f < 1920.0f; f += 128.0f)
			m_drawables.Add(new Solid(vec2(f, 1792.0f), vec2(128.0f, 112.0f), SolidType::RECT));
		for (float f = 0.0f; f < 1920.0f; f += 256.0f) {
			m_drawables.Add(new Solid(vec2(f, 1776.0f), vec2(32.0f, 16.0f), SolidType::SLOPE_R));
			m_drawables.Add(new Solid(vec2(32.0f + f, 1776.0f), vec2(16.0f, 16.0f), SolidType::RECT));
			m_drawables.Add(new Solid(vec2(48.0f + f, 1776.0f), vec2(16.0f, 16.0f), SolidType::RECT));
			m_drawables.Add(new Solid(vec2(64.0f + f, 1776.0f), vec2(32.0f, 16.0f), SolidType::SLOPE_L));
		}
		for (float f = 1920.0f; f < 3200.0f; f += 16.0f)
			m_drawables.Add(new Solid(vec2(f, 1904.0f), vec2(16.0f, 16.0f), SolidType::RECT));

		for (float f = 0.0f; f < 528.0f; f += 16.0f)
			m_drawables.Add(new Solid(vec2(992.0f + f, 368.0f + f), vec2(16.0f, 16.0f), SolidType::SLOPE_L));
	}
	else if (LEVEL_ID == 2 || LEVEL_ID == 3)
	{
		for (float f = 0.0f; f < 1920.0f; f += 128.0f)
			if(f != 256.0f)
				m_drawables.Add(new Solid(vec2(f, 640.0f), vec2(128.0f, 128.0f), SolidType::RECT));
			else m_drawables.Add(new Solid(vec2(f, 704.0f), vec2(128.0f, 64.0f), SolidType::RECT));

		m_waterHeight = 637.0f;
		LEVEL_SIZE.y = 1280.0f;
	}
	else if (LEVEL_ID == 12 || LEVEL_ID == 13)
	{
		LEVEL_SIZE.y = 1840.0f;
		for (float f = 0.0f; f < 1920.0f; f += 128.0f)
			m_drawables.Add(new Solid(vec2(f, 1776.0f), vec2(128.0f, 64.0f), SolidType::RECT));
	}
	else
	{
		for (float f = 0.0f; f < 1920.0f; f += 128.0f)
			m_drawables.Add(new Solid(vec2(f, 1776.0f), vec2(128.0f, 128.0f), SolidType::RECT));
	}
	//m_grid = new Grid(vec2(128.0f, 128.0f));
	//m_grid->Populate(&m_drawables);

	m_drawables.Add(player = new Player);

	m_drawables.Initialize();

	if (LEVEL_ID == 2u || LEVEL_ID == 3u)
		player->SetPosition(vec2(180.0f, 620.0f));

	soundemitter = new OficinaFramework::AudioSystem::AudioSource;

	OficinaFramework::ScreenSystem::Screen::Initialize();
}

void LevelScreen::LoadContent()
{
	m_drawables.LoadContent();
	bgm = [&]() -> AudioSystem::Audio*
	{
		if(LEVEL_ID == 0 || LEVEL_ID == 1)
			return OficinaFramework::AudioSystem::AudioPool::LoadAudio(
				"bgm/isolatedisland",
				OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG,
				true, 112.086f, 7.058f);
		else if (LEVEL_ID == 2 || LEVEL_ID == 3)
			return OficinaFramework::AudioSystem::AudioPool::LoadAudio(
				"bgm/amazingaquarium",
				OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG,
				true, 87.848f, 11.009f);
		else if (LEVEL_ID == 6 || LEVEL_ID == 7)
			return OficinaFramework::AudioSystem::AudioPool::LoadAudio(
				"bgm/hazelhill",
				OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG,
				true, 70.468f, 0.075f);
		else if (LEVEL_ID == 8 || LEVEL_ID == 9)
			return OficinaFramework::AudioSystem::AudioPool::LoadAudio(
				"bgm/dustydesert",
				OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG,
				true, 71.054f, 23.067f);
		else if (LEVEL_ID == 10 || LEVEL_ID == 11)
			return OficinaFramework::AudioSystem::AudioPool::LoadAudio(
				"bgm/jesterjuggle",
				OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG,
				true, 72.077f, 0.075f);
		else if (LEVEL_ID == 12 || LEVEL_ID == 13)
			return OficinaFramework::AudioSystem::AudioPool::LoadAudio(
				"bgm/chaoticcanyon",
				OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG,
				true, 78.616f, 8.799f);
		else if (LEVEL_ID == 14 || LEVEL_ID == 15)
			return OficinaFramework::AudioSystem::AudioPool::LoadAudio(
				"bgm/radiantrush",
				OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG,
				true, 98.798f, 13.530f);
		else if (LEVEL_ID == 18)
			return OficinaFramework::AudioSystem::AudioPool::LoadAudio(
				"bgm/whitewonder",
				OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG,
				true, 78.621f, 8.797f);
		else if(LEVEL_ID == 19u)
			return OficinaFramework::AudioSystem::AudioPool::LoadAudio(
				"bgm/stormysky",
				OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG,
				true, 100.072f, 6.723f);
		else if (LEVEL_ID == 20u || LEVEL_ID == 21u)
			return OficinaFramework::AudioSystem::AudioPool::LoadAudio(
				"bgm/cosmiccolony",
				OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG,
				true, 200.558f, 12.399f);
		else if(LEVEL_ID == 22u)
			return OficinaFramework::AudioSystem::AudioPool::LoadAudio(
				"bgm/orientallegend",
				OficinaFramework::AudioSystem::OF_AUDIO_TYPE_OGG,
				true, 63.999f, 12.809f);
		else if (LEVEL_ID == 23u)
		{
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
		}
		return nullptr;
	}();

	//player->setGrid(m_grid);

	// Load HUD graphics
	hudSheet = new OficinaFramework::RenderingSystem::SpriteSheet(vec2dw(58), vec2b::One(), vec2(-29.0f));
	hudSheet->AppendTexture(OficinaFramework::RenderingSystem::TexturePool::LoadTexture("sprites/headsup"));

	// Load HUD fonts
	hudFont = new RenderingSystem::Font(RenderingSystem::TexturePool::LoadTexture("fonts/hud"), vec2dw(8u, 16u), vec2b::One());
	counterFont = new RenderingSystem::Font(RenderingSystem::TexturePool::LoadTexture("fonts/counter"), vec2dw(8u), vec2b::One());
	titlecardFont = new RenderingSystem::Font(RenderingSystem::TexturePool::LoadTexture("fonts/titlecard"), vec2dw(16u), vec2b::One());

	// Parallax
	if (LEVEL_ID == 0 || LEVEL_ID == 1)
	{
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/titlescreen/parallax/layer0"),   1.0f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/titlescreen/parallax/layer1"),   0.94f, 0.98f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/titlescreen/parallax/layer2"),   0.9f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/titlescreen/parallax/layer3"),   0.92f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/titlescreen/parallax/layer4_0"), 0.9f, 0.96f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/titlescreen/parallax/layer4_1"), 0.92f));

		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/titlescreen/parallax/layer5_3"), 0.89f, 0.89f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/titlescreen/parallax/layer5_0"), 0.87f, 0.87f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/titlescreen/parallax/layer5_1"), 0.85f, 0.85f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/titlescreen/parallax/layer5_2"), 0.83f, 0.83f));
	}
	else if (LEVEL_ID == 2 || LEVEL_ID == 3)
	{
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level1/parallax/layer0"),   1.0f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level1/parallax/layer1"),   1.0f, 0.95f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level1/parallax/layer2_0"), 0.85f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level1/parallax/layer2_1"), 0.8f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level1/parallax/layer3_0"), 0.8f, 0.8f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level1/parallax/layer3_1"), 0.7f, 0.7f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level1/parallax/layer3_2"), 0.6f, 0.6f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level1/parallax/layer3_3"), 0.5f, 0.5f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level1/parallax/layer3_4"), 0.4f, 0.4f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level1/parallax/layer4_0"), 0.5f, 0.5f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level1/parallax/layer4_1"), 0.6f, 0.6f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level1/parallax/layer4_2"), 0.7f, 0.7f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level1/parallax/layer4_3"), 0.9f, 0.9f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level1/parallax/layer4_4"), 0.8f, 0.8f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level1/parallax/layer5"),   0.8f));

		fg_parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level1/parallax/fg_layer0"), 0.5f, 0.5f));
	}
	else if (LEVEL_ID == 12 || LEVEL_ID == 13)
	{
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level6/parallax/layer0"), 0.95f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level6/parallax/layer1"), 1.0f, 0.9f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level6/parallax/layer2"), 0.85f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level6/parallax/layer3"), 0.8f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level6/parallax/layer4"), 0.75f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level6/parallax/layer5"), 0.7f));
	}
	else if (LEVEL_ID == 20 || LEVEL_ID == 21)
	{
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level11/parallax/layer0"),   1.0f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level11/parallax/layer1"),   0.9f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level11/parallax/layer2_0"), 0.88f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level11/parallax/layer2_1"), 0.86f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level11/parallax/layer2_2"), 0.84f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level11/parallax/layer2_3"), 0.82f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level11/parallax/layer2_4"), 0.8f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level11/parallax/layer3_0"), 0.7f,  0.9f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level11/parallax/layer3_1"), 0.73f, 0.93f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level11/parallax/layer3_2"), 0.75f, 0.95f));
		LEVEL_SIZE.y = 2560.0f;
	}
	else if (LEVEL_ID == 22)
	{
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level12/parallax/layer0"),   1.0f));

		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level11/parallax/layer1"),   0.9f,  0.95f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level11/parallax/layer2_0"), 0.88f, 0.9f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level11/parallax/layer2_1"), 0.86f, 0.8f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level11/parallax/layer2_2"), 0.84f, 0.2f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level11/parallax/layer2_3"), 0.82f, 0.08f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level11/parallax/layer2_4"), 0.8f,  0.0002f));

		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level12/parallax/layer3_0"), 0.7f,  0.7f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level12/parallax/layer3_1"), 0.73f, 0.73f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level12/parallax/layer3_2"), 0.75f, 0.75f));

		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level12/parallax/layer4_0"), 0.3f));
		parallax.AppendPiece(new ParallaxPiece(RenderingSystem::TexturePool::LoadTexture("background/level12/parallax/layer4_1"), 0.3f));
		LEVEL_SIZE.y = 2560.0f;
	}

	if(bgm)
		soundemitter->Play(bgm);

	OficinaFramework::ScreenSystem::Screen::LoadContent();
}

void LevelScreen::UnloadContent()
{
	//delete m_grid;
	m_drawables.UnloadContent();
	m_drawables.Dispose();

	soundemitter->Stop();
	OficinaFramework::AudioSystem::AudioPool::UnloadAudio(bgm);
	delete soundemitter;

	delete hudSheet;
	delete hudFont;
	delete counterFont;
	delete titlecardFont;
	parallax.UnloadContent();
	fg_parallax.UnloadContent();

	OficinaFramework::ScreenSystem::Screen::UnloadContent();
}

void LevelScreen::Update()
{
	// Debug toggle
	if (OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_F1))
		OficinaFramework::ScreenSystem::SetDebug(!OficinaFramework::ScreenSystem::IsDebugActive());
	if (OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_F2))
		OficinaFramework::ScreenSystem::Debug_ToggleMinimalist();

	// Fullscreen toggle
	if (OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_F11))
	{
		OficinaFramework::ScreenSystem::SetFullScreen(!OficinaFramework::ScreenSystem::IsFullScreen());
		OficinaFramework::RenderingSystem::SetCameraPosition(player->GetPosition());
	}

	// Speed shoes (?)
	if (player->getState() == PLAYER_SPEEDSHOES)
		soundemitter->setPitch(2);
	else soundemitter->setPitch(1);

	// Water height
	if (m_hasWater)
		player->setWaterHeight(m_waterHeight);

	// HUD stuff
	if (m_fade > -1.0f)
	{
		if (m_titlecard_pos.x > - 56.0f)
			m_titlecard_pos.x -= 20.0f;
	}
	else if(m_titlecard_pos.x < RenderingSystem::GetResolution().x)
		m_titlecard_pos.x += 20.0f;

	// Camera
	if(!m_paused) updateCamera();

	// Parallax
	if (!m_paused)
	{
		if (LEVEL_ID == 0 || LEVEL_ID == 1)
		{
			float parallaxYpos;
			if (m_waterHeight > RenderingSystem::GetViewportPosition().y + RenderingSystem::GetResolution().y)
				parallaxYpos = 0.0f;
			else
			{
				parallaxYpos =
					(RenderingSystem::GetViewportPosition().y + RenderingSystem::GetResolution().y) - m_waterHeight;
			}
			parallax.SetYPosition(-parallaxYpos);
			fg_parallax.SetYPosition(-parallaxYpos);
		}
		else if(LEVEL_ID == 2 || LEVEL_ID == 3)
		{
			parallax.SetYPosition(-RenderingSystem::GetViewportPosition().y);
			fg_parallax.SetYPosition(-RenderingSystem::GetViewportPosition().y);
		}
		else if (LEVEL_ID == 20 || LEVEL_ID == 21 || LEVEL_ID == 22)
		{
			parallax.SetYPosition(-RenderingSystem::GetViewportPosition().y * 0.062f);
			fg_parallax.SetYPosition(-RenderingSystem::GetViewportPosition().y * 0.062f);
		}
		else
		{
			parallax.SetYPosition(0.0f);
			fg_parallax.SetYPosition(0.0f);
		}

		parallax.Update();
		fg_parallax.Update();
	}

	// Fade activation
	if (m_fade > -1.0f) m_fade -= 0.05f;

	if(m_fade > 0.0f) return;

	// Pause controls
	if (InputSystem::PressedButton(InputSystem::GamePadButton::START))
		m_paused = !m_paused;

	if (m_paused) {
		if (!soundemitter->isPaused()) soundemitter->TogglePause();

		// Back to level select
		if (InputSystem::PressedButton(InputSystem::GamePadButton::SELECT))
		{
			OficinaFramework::ScreenSystem::AddScreen(new LevelSelectScreen);
			RemoveMe();
		}
		// Do not update if paused
		return;
	}
	else {
		if (soundemitter->isPaused()) soundemitter->TogglePause();
	}

	// Timer
	if (m_timeractive) m_leveltimer++;
	dword min, seg, ds;
	min = (m_leveltimer / 3600u);
	seg = (m_leveltimer - (min * 3600u)) / 60u;
	ds  = (m_leveltimer - (min * 3600u) - (seg * 60u));
	std::ostringstream oss;
	oss.clear();
	oss << (min < 10u ? " " : "") << min << "\'"
		<< (seg < 10u ? "0" : "") << seg << "\""
		<< (ds < 10u ? "0" : "") << ds;
	m_leveltimer_hud = oss.str();

	// Debug position reset
	if (OficinaFramework::InputSystem::PressedKey(SDL_SCANCODE_F3)) {
		player->reset();
		m_cameralag = 0;
		m_playerSpindashed = false;
		OficinaFramework::RenderingSystem::SetCameraPosition(player->GetPosition());
	}

	// Drawable objects
	m_drawables.Update();
}

void LevelScreen::updateCamera()
{
	vec2 cameraPos = OficinaFramework::RenderingSystem::GetCameraPosition();

	// Debug
	if (OficinaFramework::ScreenSystem::IsDebugActive())
	{
		std::stringstream oss;
		oss.clear();
		oss << "CAM >> " << "LAG " << m_cameralag << " " << cameraPos;
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
	//vec2 desiredViewport = resolution;

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

	// Prevent camera from going beyond level limits (if existant)
	if (LEVEL_SIZE.x > resolution.x)
		if (cameraPos.x + (resolution.x / 2.0f) >= LEVEL_SIZE.x)
			cameraPos.x = LEVEL_SIZE.x - (resolution.x / 2.0f);
	if (LEVEL_SIZE.y > resolution.y)
		if (cameraPos.y + (resolution.y / 2.0f) >= LEVEL_SIZE.y)
			cameraPos.y = LEVEL_SIZE.y - (resolution.y / 2.0f);

	// Give the new position to the camera
	OficinaFramework::RenderingSystem::SetCameraPosition(cameraPos);
}

void LevelScreen::Draw()
{
	vec2 viewportPos = OficinaFramework::RenderingSystem::GetViewportPosition();
	vec2 viewportSize = OficinaFramework::RenderingSystem::GetViewportSize().toVec2();
	vec2 resolution = OficinaFramework::RenderingSystem::GetResolution().toVec2();

	if (!m_clearcolorset)
	{
		if (LEVEL_ID == 0 || LEVEL_ID == 1)
			//OficinaFramework::RenderingSystem::glClearColorM(BLUEVIOLET);
			glClearColor(0.25f, 0.125f, 0.375f, 1.0f);
		else if (LEVEL_ID == 2 || LEVEL_ID == 3)
		{
			glClearColor(0.21875f, 0.40625f, 0.78125f, 1.0f);
		}
		else if(LEVEL_ID == 22u)
			glClearColor(0.25f, 0.0f, 0.1f, 1.0f); // Dark red
		else if(LEVEL_ID == 23u)
		{
			switch (dayTime) {
			default:
			case DAYTIME_MORNING:   OficinaFramework::RenderingSystem::glClearColorM(CORNFLOWERBLUE);  break;
			case DAYTIME_AFTERNOON: OficinaFramework::RenderingSystem::glClearColorM(ORANGE);          break;
			case DAYTIME_EVENING:   glClearColor(0.1f, 0.0f, 0.25f, 1.0f);                             break;
			case DAYTIME_NIGHT:     OficinaFramework::RenderingSystem::glClearColorM(DARKBLUE);        break;
			}
		}
		else RenderingSystem::glClearColorM(GREY50);
		m_clearcolorset = true;
	}

	// Background
	parallax.Draw();

	// Drawables
	m_drawables.Draw();

	// Foreground
	fg_parallax.Draw();

	// Water
	if (m_hasWater)
	{
		if (m_waterHeight <= viewportPos.y + resolution.y)
		{
			float waterTop = m_waterHeight > viewportPos.y ? m_waterHeight - viewportPos.y : 0.0f;

			glPushMatrix();
			glTranslatef(viewportPos.x, viewportPos.y, 0.0f);
			glBegin(GL_QUADS);
			if (LEVEL_ID == 0 || LEVEL_ID == 1)
				glColor4f(0.25f, 0.125f, 0.375f, 0.6f);
			else if (LEVEL_ID == 2 || LEVEL_ID == 3)
				OficinaFramework::RenderingSystem::glColorM(DARKBLUE, 0.5078125f);
			else if (LEVEL_ID == 25)
				OficinaFramework::RenderingSystem::glColorM(BLACK, 0.6f);
			else
				OficinaFramework::RenderingSystem::glColorM(DARKBLUE, 0.4f);
			glVertex2f(0.0f, waterTop);
			glVertex2f(resolution.x, waterTop);
			glVertex2f(resolution.x, resolution.y);
			glVertex2f(0.0f, resolution.y);
			glEnd();
			glPopMatrix();
		}
	}

	// Heads-up display
	glPushMatrix();
		OficinaFramework::RenderingSystem::glTranslateToViewportPos();
		glTranslatef(resolution.x / 32.0f, 0.0f, 0.0f);

		// Stats hud
		glPushMatrix();
			glTranslatef(0.0f, resolution.y / 32.0f, 0.0f);
				hudSheet->DrawFrame(vec2::Zero(), 1);
				glPushMatrix();
					glTranslatef(45.0f, 1.0f, 0.0f);

					// Score
					hudFont->DrawString(vec2::Zero(), "       0");

					// Time
					glTranslatef(0.0f, 17.0f, 0.0f);
					hudFont->DrawString(vec2::Zero(), m_leveltimer_hud);

					// Rings
					glTranslatef(0.0f, 17.0f, 0.0f);
					hudFont->DrawString(vec2::Zero(), "  0");
				glPopMatrix();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0.0f, (31.0f * (resolution.y / 32.0f)) - 16.0f, 0.0f);
			// Lives
			hudSheet->DrawFrame(vec2::Zero(), 2);

			// Lives counter
			counterFont->DrawString(vec2(40.0f, 10.0f), "3");
		glPopMatrix();
	glPopMatrix();

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
	if (LEVEL_ID == 25u && player->GetPosition().y > 100000.0f)
	{
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
	}

	// Title Card
	if (m_fade > -1.0f || m_titlecard_pos.x < RenderingSystem::GetResolution().x)
	{
		glPushMatrix();
			OficinaFramework::RenderingSystem::glTranslateToViewportPos();

			// Background & Act
			glPushMatrix();
				glTranslatef((5.0f * (RenderingSystem::GetResolution().x / 8.0f)) + m_titlecard_pos.x, m_titlecard_pos.y, 0.0f);
				hudSheet->DrawFrame(vec2::Zero(), 0);
				if(LEVEL_ACT != 0)
					hudSheet->DrawFrame(vec2(-12.0f, 8.0f), LEVEL_ACT + 2u);
			glPopMatrix();

			// Level Name
			glPushMatrix();
				glTranslatef((RenderingSystem::GetResolution().x / 2.0f) - m_titlecard_pos.x, m_titlecard_pos.y + 8.0f, 0.0f);

				glPushMatrix();
					glTranslatef(-titlecardFont->MeasureString(LEVEL_NAME, 1.0f).x, 0.0f, 0.0f);
					titlecardFont->DrawString(vec2::Zero(), LEVEL_NAME);
				glPopMatrix();

				// Zone / Fight
				if (LEVEL_ID != 22)
				{
					glPushMatrix();
						glTranslatef(-titlecardFont->MeasureString("ZONE", 1.0f).x - 16.0f, 20.0f, 0.0f);
						titlecardFont->DrawString(vec2::Zero(), "ZONE");
					glPopMatrix();
				}
				else
				{
					glPushMatrix();
						glTranslatef(-titlecardFont->MeasureString("FIGHT", 1.0f).x - 16.0f, 20.0f, 0.0f);
						titlecardFont->DrawString(vec2::Zero(), "FIGHT");
					glPopMatrix();
				}

			glPopMatrix();

		glPopMatrix();
	}
}
