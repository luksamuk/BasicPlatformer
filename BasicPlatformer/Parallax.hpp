#pragma once
#include <OficinaFramework\RenderingSystem.hpp>
#include <OficinaFramework\EntitySystem.hpp>
using namespace OficinaFramework;

struct ParallaxPiece
{
	float* Xpositions = nullptr;
	float Yposition = 0.0f;
	RenderingSystem::Texture* t = nullptr;
	float followRatio = 1.0f;
	dword amount = 0u;
	bool autoWalk = false;

	ParallaxPiece(RenderingSystem::Texture* t, float followRatio = 1.0f, bool autoWalk = false, vec2 position = 0.0f);

	~ParallaxPiece();
};

class Parallax
{
private:
	std::vector<ParallaxPiece*> pieces;
	float yPos;
public:
	void Update();
	void UnloadContent();
	void Draw();

	void AppendPiece(ParallaxPiece*);

	float GetYPosition() const;
	void SetYPosition(float);
};