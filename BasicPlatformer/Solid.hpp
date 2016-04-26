#pragma once
#include <OficinaFramework\EntitySystem.hpp>

enum class SolidType
{
	RECT,
	SLOPE_L,
	SLOPE_R
};


class Solid :
	public OficinaFramework::EntitySystem::DrawableEntity
{
private:
	vec2 m_size;
	SolidType type;
public:
	Solid(vec2 position, vec2 size, SolidType type);
	~Solid();

	// Inherited via DrawableEntity
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LoadContent() override;
	virtual void UnloadContent() override;
	virtual void Draw() override;

	float angularCoefficient(float XPosition);

	float Top(float XPosition);
	float Left();
	float Right();
	float Bottom();

	vec2 Middle();

	int getType();

	bool Intersects(vec2 Position, vec2 size);
	bool Linecast(vec2 Position, vec2 direction, float size);
};

