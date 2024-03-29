#include <OficinaFramework/OficinaFramework.hpp>
#include <list>
#include <string>
using namespace OficinaFramework;

#include "LoadingScreen.hpp"
#include "PresentationScreen.hpp"

int main(int argc, char** argv)
{
	// Remember to have folders assets/ and resources/ inside your
	// project. Make sure prepare_assets.bat is inside resources/
	// folder.

	std::list<std::string>* confv = new std::list<std::string>;
	confv->push_back("gamename=SONIC THE HEDGEHOG OF");
	confv->push_back("framerate=60hz");
	confv->push_front("vsync");
	confv->push_back("resolution=640x360"); // Recommended
	confv->push_back("iconpath=icon");
	confv->push_front("enable_diagnostics");
	

	EngineCore::Initialize(argc, argv, confv);
	RenderingSystem::SetLinearFiltering(false);
	RenderingSystem::glClearColorM(BLACK);
	InputSystem::BindDefaultKeys();
	ScreenSystem::Debug_SetFont(new RenderingSystem::Font(RenderingSystem::TexturePool::LoadTexture("fonts/debugger"), vec2dw(8u), vec2b::One()));

	// Set default window size to 720p
	//ScreenSystem::SetWindowSize(vec2dw(1280, 720));
	//RenderingSystem::DestroyDefaultBuffer();
	//RenderingSystem::SetViewportSize(ScreenSystem::GetWindowSize());
	//RenderingSystem::SetResolution(RenderingSystem::GetResolution());
	//RenderingSystem::CreateDefaultBuffer();
	
	ScreenSystem::AddScreen(new LoadingScreen);
	ScreenSystem::AddScreen(new PresentationScreen);

	return EngineCore::DoGameLoop();
}
