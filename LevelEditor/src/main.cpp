#include <OficinaFramework/OficinaFramework.hpp>
#include "EditorScreen.hpp"
#include <string>
#include <list>
using namespace OficinaFramework;

int main(int argc, char** argv)
{
	std::list<std::string>* confv = new std::list<std::string>;
	confv->push_back("gamename=ImGui OficinaFramework Test");
	confv->push_back("framerate=60Hz");
	confv->push_back("resolution=720p");

	EngineCore::Initialize(argc, argv, confv);
	//RenderingSystem::glClearColorM(BLACK);
	ScreenSystem::Debug_SetFont(RenderingSystem::TexturePool::LoadDefaultFont());
	InputSystem::BindDefaultKeys();

	//ScreenSystem::SetDebug(true);
	RenderingSystem::SetCameraPosition(RenderingSystem::GetResolution().toVec2() / 2.0f);
	ScreenSystem::AddScreen(new EditorScreen);

	return EngineCore::DoGameLoop();
}
